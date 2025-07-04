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

enum Direction {
	CENTER = 0,
	UP = 1,
	DOWN = -1,
	LEFT = 2,
	RIGHT = -2,
	FORWARD = 3,
	BACKWARD = -3
};

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
		void moveObject(int direction);
		[[nodiscard]] const std::string& getFilename() const;
		[[nodiscard]] const Vec3& getPosition() const;
    
   	private:
		ObjectData() = default;
		~ObjectData() = default;
		std::string filename;
		std::vector<Vec3> vertices;
		std::vector<unsigned int> faces;
		std::vector<VertexAttrib> attributes; // Attributes for each vertex, including position and color
		std::vector<unsigned int> indices;
		Vec3 position{0.0f, 0.0f, 0.0f};
		Vec3 center{0.0f, 0.0f, 0.0f}; // Center of the object
		size_t lineIndex = 0; // For error reporting
		void getFace(std::istringstream& iss);
		void computeCenter();
		void computeAttributes();
};

#endif //OBJECTDATA_HPP
