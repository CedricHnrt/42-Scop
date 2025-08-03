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

static void skipCommentsAndWS(std::ifstream& file)
{
	while (true) {
		const char c = static_cast<char>(file.peek());
		if (c == '#') {
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip comments
		} else if (!std::isspace(c)) {
			file.get(); // Skip whitespace
		}
		else {
			break;
		}
	}
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
		constexpr float angle = -M_PI / 2.0f;
		for (int j = 0; j < 3; ++j) {
			const unsigned int index = this->faces[i + j]; 
			const Vec3 vertex = this->vertices[index]; 
			VertexAttrib attrib; 
			attrib.position = vertex; 
			attrib.color = Vec3(shade, shade, shade);

			const float scale = std::max(this->maxY - this->minY, this->maxZ - this->minZ);
			float u = (vertex.y - this->minY) / (this->maxY - this->minY) * scale;
			float v = (vertex.z - this->minZ) / (this->maxZ - this->minZ) * scale;

			float uCentered = u - 0.5f;
			float vCentered = v - 0.5f;
			float uRotated = uCentered * std::cos(angle) - vCentered * std::sin(angle);
			float vRotated = uCentered * std::sin(angle) + vCentered * std::cos(angle);
			u = uRotated + 0.5f;
			v = vRotated + 0.5f;
			
			attrib.texCoord = Vec2(u, v);
			
			this->attributes.push_back(attrib); 
			this->indices.push_back(this->indices.size()); // Maintain a flat index for OpenGL
		}
	}
}

void ObjectData::computeUVBound()
{
	for (const Vec3& v : this->vertices) {
		this->minX = std::min(this->minX, v.x);
		this->maxX = std::max(this->maxX, v.x);
		this->minZ = std::min(this->minZ, v.z);
		this->maxZ = std::max(this->maxZ, v.z);
		this->minY = std::min(this->minY, v.y);
		this->maxY = std::max(this->maxY, v.y);
	}
}

void ObjectData::load(const char* filepath) {
	checkFilename(filepath);
	this->filename = prepareFilename(filepath); // Extract filename from path
	
	std::cout << BOLD << "Loading " << this->filename << "..." << RESET << std::endl;
	std::ifstream file(filepath);
	if (!file.is_open())
		throw UnableToOpenOBJException();
	
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
	this->computeUVBound();
	this->computeAttributes();
	std::cout << GREEN << BOLD << this->filename << " loaded succesfully." << RESET << std::endl;
	std::cout << std::endl;
	this->printInfo();
}

void ObjectData::draw() {
	if (this->showTexture && this->transitionFactor < 1.0f)
		this->transitionFactor = std::min(1.0f, this->transitionFactor + FrameTimer::getInstance().getDeltaTime() * 0.75f);
	else if (!this->showTexture && this->transitionFactor > 0.0f)
		this->transitionFactor = std::max(0.0f, this->transitionFactor - FrameTimer::getInstance().getDeltaTime() * 0.75f);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VertexAttrib), &this->attributes[0].texCoord);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, sizeof(VertexAttrib), &this->attributes[0].color);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_CONSTANT);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
	const GLfloat envColor[4] = {1.0f, 1.0f, 1.0f, this->transitionFactor};
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, envColor);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(VertexAttrib), &this->attributes[0].position);

	glDrawElements(GL_TRIANGLES, static_cast<int>(this->indices.size()), GL_UNSIGNED_INT, this->indices.data());

	glDisableClientState(GL_VERTEX_ARRAY);
	if (this->showTexture) {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	} else {
		glDisableClientState(GL_COLOR_ARRAY);
	}
}

void ObjectData::dataToOpenGL()
{
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->ppmData.width, this->ppmData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->ppmData.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void ObjectData::loadPPM(const char* filepath) {
	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		throw UnableToOpenPPMException(filepath);
	}
	std::string format;
	file >> format;
	if (format != "P6") {
		throw WrongPPMFormatException(filepath);
	}
	skipCommentsAndWS(file); // Skip comments and whitespace after the format
	int width, height;
	file >> width >> height;
	if (width <= 0 || height <= 0) {
		throw WrongPPMFormatException(filepath);
	}
	skipCommentsAndWS(file); // Skip comments and whitespace after dimensions
	int maxColorValue;
	file >> maxColorValue;
	if (maxColorValue != 255) {
		throw WrongPPMFormatException(filepath);
	}
	this->ppmData.data = new unsigned char[width * height * 3]; // Allocate memory for pixel data
	skipCommentsAndWS(file); // Skip any remaining whitespace before reading pixel data
	file.read(reinterpret_cast<char*>(this->ppmData.data), width * height * 3);
	if (!file) {
		delete[] this->ppmData.data;
		throw WrongPPMFormatException(filepath);
	}
	for (int i = 0; i < width * height * 3; i += 3) {
		std::swap(this->ppmData.data[i + 1], this->ppmData.data[i + 2]); // swap G et B
	}
	this->ppmData.width = width;
	this->ppmData.height = height;
	this->dataToOpenGL();
	std::cout << GREEN << BOLD << "PPM texture loaded successfully from " << filepath << RESET << std::endl;
	std::cout << std::endl;
	file.close();
}

void ObjectData::moveObject(const int direction, const float speed) {
	switch (direction) {
		case CENTER:
			this->position = Vec3(0.0f, 0.0f, 0.0f);
			break;
		case UP:
			this->position.y += speed * FrameTimer::getInstance().getDeltaTime();
			break;
		case DOWN:
			this->position.y -= speed * FrameTimer::getInstance().getDeltaTime();
			break;
		case LEFT:
			this->position.x -= speed * FrameTimer::getInstance().getDeltaTime();
			break;
		case RIGHT:
			this->position.x += speed * FrameTimer::getInstance().getDeltaTime();
			break;
		case FORWARD:
			this->position.z += speed * FrameTimer::getInstance().getDeltaTime();
			break;
		case BACKWARD:
			this->position.z -= speed * FrameTimer::getInstance().getDeltaTime();
			break;
		default:
			break;
	}
}

void ObjectData::toggleTexture() {
	this->showTexture = !this->showTexture;
}

void ObjectData::printInfo() const {
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

ObjectData::~ObjectData() {
	if (this->ppmData.data) {
		delete[] this->ppmData.data;
		this->ppmData.data = nullptr;
	}
}