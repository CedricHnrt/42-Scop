#include "ObjectData.hpp"

static void checkFilename(const char* filename) {
	if (filename == nullptr || filename[0] == '\0')
		throw NoArgException();
	std::string file(filename);
	if (file.size() < 5 || file.substr(file.size() - 4) != ".obj" )
		throw WrongExtensionException();
}

ObjectData& ObjectData::getInstance() {
	static ObjectData instance;
	return instance;
}

void ObjectData::load(const char* filename) {
	checkFilename(filename);
	object = filename;
	std::cout << "Loading " << filename << std::endl;
	//TODO: Parse obj file & store data
}

void ObjectData::printInfo() const {
	std::cout << "Object file: " << object << std::endl;
	//TODO: Print obj file data
}