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

void ObjectData::load(const char* filepath) {
	checkFilename(filepath);
	this->filename = prepareFilename(filepath); // Extract filename from path
	
	std::cout << BOLD << "Loading " << this->filename << "..." << RESET << std::endl;
	std::ifstream file(filepath);
	if (!file.is_open())
		throw UnableToOpenFileException();
	
	std::string line;
	size_t lineIndex = 0; // Line index for error reporting
	while (std::getline(file, line)) {
		lineIndex++;
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
			std::vector<int> face;
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
					std::cerr << "Line " << lineIndex << std::endl;
					face.clear();
					break;
				}
				catch (const std::out_of_range& e) {
					std::cerr << YELLOW << "WARNING: Vertex index out of range: " << vIndexString << RESET << std::endl;
					std::cerr << "Line " << lineIndex << std::endl;
					face.clear();
					break;
				}
			}
			if (face.size() >= 3) // Ensure at least a triangle
				faces.push_back(face);
		}
	}
	file.close();
	std::cout << GREEN << BOLD << this->filename << " loaded succesfully." << RESET << std::endl;
	this->printInfo();
}

void ObjectData::printInfo() const {
	std::cout << std::endl;
	std::cout << "Object file: " << this->filename << std::endl;
	std::cout << "Vertices: " << this->vertices.size() << std::endl;
	std::cout << "Faces: " << this->faces.size() << std::endl;
	std::cout << std::endl;
}

const std::string& ObjectData::getFilename() const {
	return this->filename;
}


ObjectData& ObjectData::getInstance() {
	static ObjectData instance;
	return instance;
}