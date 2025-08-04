#ifndef OBJECTDATA_HPP
#define OBJECTDATA_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <GL/gl.h>
#include "exceptionTypes.hpp"
#include "ansiCodes.hpp"
#include "matrix.hpp"
#include "FrameTimer.hpp"

#define TEX_PATH "assets/textures/texture.ppm"

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
	Vec2 texCoord;
};

struct PPMData {
	int width;
	int height;
	unsigned char* data;
};

class ObjectData {
  	public:
    	static ObjectData& getInstance();
		ObjectData(const ObjectData&) = delete;
		ObjectData& operator=(const ObjectData&) = delete;
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
		void load(const char* filepath);
		void loadPPM(const char *filepath);
		void draw();
		void printInfo() const;
		void moveObject(int direction, float speed = 3.0f);
		void toggleTexture();
		[[nodiscard]] const std::string& getFilename() const;
		[[nodiscard]] const Vec3& getPosition() const;
    
   	private:
		ObjectData() = default;
		~ObjectData();
		std::string filename;
		std::vector<Vec3> vertices;
		std::vector<unsigned int> faces;
		std::vector<VertexAttrib> attributes; // Attributes for each vertex, including position and color
		std::vector<unsigned int> indices;
		Vec3 position{0.0f, 0.0f, 0.0f};
		Vec3 center{0.0f, 0.0f, 0.0f}; // Center of the object
		size_t lineIndex = 0; // For error reporting
		PPMData ppmData{};
		GLuint textureID = 0;
		float minX = +INFINITY, minZ = +INFINITY, minY = +INFINITY;
		float maxX = -INFINITY, maxZ = -INFINITY, maxY = -INFINITY;
		bool showTexture = false;
		float transitionFactor = 0.0f; // For texture transition
		void getFace(std::istringstream& iss);
		void computeCenter();
		void computeAttributes();
		void computeUVBound();
		void dataToOpenGL();
};

#endif //OBJECTDATA_HPP
