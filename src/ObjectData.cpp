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
		const float angle = -M_PI / 2.0f;
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
			this->indices.push_back(this->indices.size()); // Maintain 