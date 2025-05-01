#include "ObjectData.hpp"

static void checkFilename(const char* filename) {
	if (filename == nullptr || filename[0] == '\0')
		throw NoArgException();
	const std::string file(filename);
	if (file.size() < 5 || file.substr(file.size() - 4) != ".obj" )
		throw WrongExtensionException();
}

static std::string getFilename(const std::string& filepath) {
	return filepath.substr(filepath.find_last_of("\\/") + 1);
}

void ObjectData::load(const char* filepath) {
	checkFilename(filepath);
	this->filename = getFilename(filepath);
	
	std::cout << BOLD << "Loading " << this->filename << "..." << RESET << std::endl;
	std::ifstream file(filepath);
	if (!file.is_open())
		throw UnableToOpenFileException();
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#')
			continue;
		std::istringstream iss(line);
		std::string type;
		iss >> type;
		if (type == "v") {
			Vec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			this->vertices.push_back(vertex);
		}
		else if (type == "f") {
			std::vector<int> face;
			std::string part;
			while (iss >> part) {
				std::istringstream pss(part);
				std::string vIndexString;
				std::getline(pss, vIndexString, '/');
				face.push_back(std::stoi(vIndexString) - 1);
			}
			faces.push_back(face);
		}
	}
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

ObjectData& ObjectData::getInstance() {
	static ObjectData instance;
	return instance;
}