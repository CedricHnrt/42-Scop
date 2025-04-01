#include <iostream>
#include "exceptionTypes.hpp"
#include "ObjectData.hpp"

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

		//TODO: Handle window

		//TODO: Create animation loop, controls...
		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return errorCode;
	}
}