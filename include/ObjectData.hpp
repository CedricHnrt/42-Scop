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

struct VertexAttrib {
	Vec3 position;
	Vec3 color;
};

class ObjectData {
  	public:
    	static ObjectData& getInstance();
		ObjectData(const ObjectData&) = delete;
		ObjectData& operator=(const ObjectData&) = delete;
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
		void load(const char* filepath);
		void draw() const;
		void printInfo() const;
		[[nodiscard]] const std::string& getFilename() const;
    
   	private:
		ObjectData() = default;
		~ObjectData() = default;
		std::string filename;
		std::vector<Vec3> vertices;
		std::vector<unsigned int> faces;
		std::vector<VertexAttrib> attributes; // Attributes fo