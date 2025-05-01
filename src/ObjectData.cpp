#include "ObjectData.hpp"

static void checkFilename(const char* filename) {
	if (filename == nullptr || filename[0] == '\0')
		throw NoArgException();
	std::string file(filename);
	if (file.size() < 5 || file.substr(file.size() - 4) != ".obj" )
		throw WrongExtensionException();
}

static std::string getFilename(const std::string& filename) {
	return filename.substr(filename.find_last_of("\\/") + 1);
}

void ObjectData::load(const char* filename) {
	checkFilename(filename);
	this->filename = getFilename(filename);
	std::cout << "Loading " << filename << std::endl;
	//TODO: Parse obj file & store data
	this->printInfo();
}

void ObjectData::printInfo() const {
	std::cout << "Object file: " << this->filename << std::endl;
	//TODO: Print obj file data
}

ObjectData& ObjectData::getInstance() {
	static ObjectData instance;
	return instance;
}