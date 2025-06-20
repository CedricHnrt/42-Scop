#include <iostream>
#include "exceptionTypes.hpp"
#include "ObjectData.hpp"
#include "WindowManager.hpp"

errorType errorCode = NO_ERROR;

int main(const int argc, const char *argv[])
{
	try {
		if (argc != 2) {
			if (argc == 1)
				throw NoArgException();
			throw TooManyArgException();
		}
		ObjectData::getInstance().load(argv[1]);
		WindowManager::getInstance().createWindow();
		WindowManager::getInstance().animationLoop();
		//TODO: Create animation loop, controls...
		
	}
	catch (const std::exception& e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		errorCode = errorCode == NO_ERROR ? UNDEFINED_ERROR : errorCode;
	}
	return errorCode;
}