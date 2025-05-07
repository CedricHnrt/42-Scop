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
	void createWindow(const char* name = nullptr);	// default name to "SCOP - <filename>"

private:
	WindowManager() = default; 
	~WindowManager();
};

#endif //WINDOWMANAGER_HPP
