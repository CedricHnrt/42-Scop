#include "ObjectData.hpp"

static void checkFilename(const char* filename) {
	if (filename == nullptr || filename[0] == '\0')
		throw NoArgException();
	const std::string file(filename);
	if (file.size() < 5 || file.substr(file.size() - 4) != ".obj" )
		throw WrongExtensionException();
}

static std::string prepareFilename(const std::string& filepath) {
	return filepath.substr(filepath.find_last_of("\\/") + 1);
}

void ObjectData::getFace(std::istringstream& iss) {
	std::vector<unsigned int> face;
	std::string part;
	while (iss >> part) { // Read each part of the face definition
		std::istringstream pss(part);
		std::string vIndexString;
		std::getline(pss, vIndexString, '/'); // Ignore texture and normal indices
		try {
			face.push_back(std::stoi(vIndexString) - 1); // OBJ indices are 1-based
		}
		catch (const std::invalid_argument& e) {
			std::cerr << YELLOW << "WARNING: Invalid vertex index: " << vIndexString << RESET << std::endl;
			std::cerr << "Line " << this->lineIndex << std::endl;
			face.clear();
			break;
		}
		catch (const std::out_of_range& e) {
			std::cerr << YELLOW << "WARNING: Vertex index out of range: " << vIndexString << RESET << std::endl;
			std::cerr << "Line " << this->lineIndex << std::endl;
			face.clear();
			break;
		}
	}
	if (face.size() == 3) {
		this->faces.insert(this->faces.end(), face.begin(), face.end()); // Directly add triangle indices
	}
	else if (face.size() < 3) { // Ensure at least a triangle
		std::cerr << YELLOW << "WARNING: Face with less than 3 vertices found, skipping." << RESET << std::endl;
		std::cerr << "Line " << this->lineIndex << std::endl;
	}
	else { // Handle polygons with more than 3 vertices
		// Triangulate the polygon
		for (size_t i = 1; i < face.size() - 1; ++i) {
			std::vector triangle = {face[0], face[i], face[i + 1]};
			this->faces.insert(this->faces.end(), triangle.begin(), triangle.end());
		}
	}
}

void ObjectData::computeCenter() {
	for (const auto& vertex : this->vertices) {
		this->center = vertex + this->center;
	}
	this->center /= static_cast<float>(this->vertices.size()); // Average to find the center
	for (auto& vertex : this->vertices) {
		vertex = vertex - this->center; // Center the vertices around the origin
	}
}

void ObjectData::computeAttributes() {
	for (unsigned int i = 0; i < this->faces.size(); i += 3) { 
		const float shade = (i / 3 % 10) / 10.0f; 
		for (int j = 0; j < 3; ++j) {
			const unsigned int index = this->faces[i + j]; 
			const Vec3 vertex = this->vertices[index]; 
			VertexAttrib attrib; 
			attrib.position = vertex; 
			attrib.color = Vec3(shade, shade, shade);
			this->attributes.push_back(attrib); 
			this->indices.push_back(this->indices.size()); // Maintain a flat index for OpenGL
		}
	}
}

void ObjectData::load(const char* filepath) {
	checkFilename(filepath);
	this->filename = prepareFilename(filepath); // Extract filename from path
	
	std::cout << BOLD << "Loading " << this->filename << "..." << RESET << std::endl;
	std::ifstream file(filepath);
	if (!file.is_open())
		throw UnableToOpenFileException();
	
	std::string line;
	while (std::getline(file, line)) {
		this->lineIndex++;
		if (line.empty() || line[0] == '#')
			continue;
		std::istringstream iss(line);
		std::string type;
		iss >> type;
		if (type == "v") {	//Vertex coordinates
			Vec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			this->vertices.push_back(vertex);
		}
		else if (type == "f") {	//Face indices
			this->getFace(iss);
		}
	}
	file.close();
	glEnableClientState(GL_VERTEX_ARRAY); // Enable vertex array functionality
	this->computeCenter();
	this->computeAttributes();
	std::cout << GREEN << BOLD << this->filename << " loaded succesfully." << RESET << std::endl;
	this->printInfo();
}

void ObjectData::draw() const {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, sizeof(VertexAttrib), &this->attributes[0].position); // Set vertex pointer to the position attribute
	glColorPointer(3, GL_FLOAT, sizeof(VertexAttrib), &this->attributes[0].color); // Set color pointer to the color attribute
	
	glDrawElements(GL_TRIANGLES, static_cast<int>(this->indices.size()), GL_UNSIGNED_INT, this->indices.data()); // Draw the elements using the flat indices
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ObjectData::moveObject(const int direction) {
	switch (direction) {
		case CENTER:
			this->position = Vec3(0.0f, 0.0f, 0.0f);
			break;
		case UP:
			this->position.y += 3.00f * FrameTimer::getInstance().getDeltaTime();
			break;
		case DOWN:
			this->position.y -= 3.00f * FrameTimer::getInstance().getDeltaTime();
			break;
		case LEFT:
			this->position.x -= 3.00f * FrameTimer::getInstance().getDeltaTime();
			break;
		case RIGHT:
			this->position.x += 3.00f * FrameTimer::getInstance().getDeltaTime();
			break;
		case FORWARD:
			this->position.z += 3.00f * FrameTimer::getInstance().getDeltaTime();
			break;
		case BACKWARD:
			this->position.z -= 3.00f * FrameTimer::getInstance().getDeltaTime();
			break;
		default:
			break;
	}
}


void ObjectData::printInfo() const {
	std::cout << std::endl;
	std::cout << "Object file: " << this->filename << std::endl;
	std::cout << "Vertices: " << this->vertices.size() << std::endl;
	std::cout << "Faces: " << this->faces.size() / 3 << std::endl;
	std::cout << std::endl;
}

const std::string& ObjectData::getFilename() const {
	return this->filename;
}

const Vec3& ObjectData::getPosition() const {
	return this->position;
}

ObjectData& ObjectData::getInstance() {
	static ObjectData instance;
	return instance;
}