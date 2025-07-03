#ifndef OBJECTDATA_HPP
#define OBJECTDATA_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "exceptionTypes.hpp"
#include "ansiCodes.hpp"
#include "matrix.hpp"
#include <GL/gl.h>


class ObjectData {
  	public:
    	static ObjectData& getInstance();
		ObjectData(const ObjectData&) = delete;
		ObjectData& operator=(const ObjectData&) = delete;
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
		void load(const char* filepath);
		void draw();
		void printInfo() const;
		[[nodiscard]] const std::string& getFilename() const;
    
   	private:
		ObjectData() = default;
		~ObjectData() = default;
		std::string filename;
		std::vector<Vec3> vertices;
		std::vector<unsigned int> faces;
		Vec3 center{0.0f, 0.0f, 0.0f}; // Center of the object
		size_t lineIndex = 0; // For error reporting
		void getFace(std::istringstream& iss);
		void computeCenter();
};

#endif //OBJECTDATA_HPP
