#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <string>
#include "ObjectData.hpp"
#include "matrix.hpp"
#include "FrameTimer.hpp"

class WindowManager {
public:
	static WindowManager& getInstance();
	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;
	void* operator new(size_t) = delete;
	void operator delete(void*) = delete;
	void createWindow(const char *name = nullptr, const std::vector<int>& windowRes = std::vector<int>());
	void loop();

private:
	Display* display = nullptr;
	Window window{};
	Mat4 projectionMatrix = Mat4::identity();
	Mat4 viewMatrix = Mat4::identity();
	Mat4 modelMatrix = Mat4::identity();
	float rotationAngle = 0.0f; // For rotation animation
	long wmDelete = None;
	bool running = false;
	bool paused = false;
	int screen = 0;
	std::string name;
	std::vector<int> resolution;
	void resolveName(const char *name);
	void resolveResolution(const std::vector<int>& windowRes);
	void updateProjectionMatrix();
	void render();
	WindowManager() = default; 
	~WindowManager();
};

#endif //WINDOWMANAGER_HPP
