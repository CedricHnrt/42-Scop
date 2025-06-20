#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <string>
#include "ObjectData.hpp"

class WindowManager {
public:
	static WindowManager& getInstance();
	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;
	void* operator new(size_t) = delete;
	void operator delete(void*) = delete;
	void createWindow(const char *name = nullptr, const std::vector<int>& windowRes = std::vector<int>());
	void animationLoop();

private:
	Display* display = nullptr;
	Window window;
	long wmDelete = None;
	bool running = false;
	int screen = 0;
	std::string name;
	std::vector<int> resolution;
	void resolveName(const char *name);
	void resolveResolution(const std::vector<int>& windowRes);
	WindowManager() = default; 
	~WindowManager();
};

#endif //WINDOWMANAGER_HPP
