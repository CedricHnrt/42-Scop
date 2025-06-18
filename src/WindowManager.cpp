#include "WindowManager.hpp"

void WindowManager::resolveName(const char *name) {
	if (name)
		this->name = name;
	this->name = "SCOP - " + ObjectData::getInstance().getFilename();
}

static bool validateResolution(const std::vector<int>& windowRes, const std::vector<int>& maxRes) {
	if (windowRes.size() != 2)
		return false;
	if (windowRes[0] > maxRes[0] || windowRes[1] > maxRes[1])
		return false;
	if (windowRes[0] < 800 || windowRes[1] < 600)
		return false;
	return true;
}

void WindowManager::resolveResolution(const std::vector<int>& windowRes) {
	std::vector<int> screenRes;	// Default screen resolution
	screenRes.push_back(XDisplayWidth(this->display, this->screen));
	screenRes.push_back(XDisplayHeight(this->display, this->screen));
	if (!windowRes.empty() && validateResolution(windowRes, screenRes)) // Validate the provided resolution
		this->resolution = windowRes;
	else
		this->resolution = screenRes;
	std::cout << "Resolution: " << this->resolution[0] << ", " << this->resolution[1] << std::endl; // REMOVE
}

void WindowManager::createWindow(const char *name, const std::vector<int>& windowRes) {
	this->display = XOpenDisplay(nullptr); // Open the default X display
	if (!display) {
		throw std::runtime_error("Failed to open X display");
	}
	this->screen = DefaultScreen(this->display); // Get the default screen

	this->resolveName(name); // Set Window Name
	this->resolveResolution(windowRes); // Set Window Resolution
	
	int visualAttribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None}; // Attributes for the visual
	XVisualInfo* visual = glXChooseVisual(this->display, this->screen, visualAttribs); // Choose a visual that supports OpenGL
	if (!visual) {
		throw std::runtime_error("Failed to get a visual from X screen");
	}
	const Window root = RootWindow(this->display, this->screen); // Get the root window of the screen
	Colormap colormap = XCreateColormap(this->display, root, visual->visual, AllocNone); // Create a colormap for the visual
	XSetWindowAttributes attributes;
	attributes.colormap = colormap;
	attributes.event_mask = ExposureMask | KeyPressMask;
	
	this->window = XCreateWindow(this->display, root, 0, 0, this->resolution[0], this->resolution[1], 0,
			visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask, &attributes); // Create the window

	XStoreName(this->display, this->window, this->name.c_str()); // Set the window name
	XMapWindow(this->display, this->window); // Map the window to the screen
	GLXContext context = glXCreateContext(this->display, visual, nullptr, GL_TRUE);
	if (!context) {
		throw std::runtime_error("Failed to create OpenGL context");
	}
	glXMakeCurrent(this->display, this->window, context); // Make the context current
}

WindowManager& WindowManager::getInstance() {
	static WindowManager instance;
	return instance;
}

WindowManager::~WindowManager() {
	glXMakeCurrent(this->display, None, nullptr);
	if (this->display && this->screen >= 0) {
		XFreeColormap(this->display, DefaultColormap(this->display, this->screen)); // Free the colormap
	}
	XDestroyWindow(this->display, this->window);
	if (this->display)
		XCloseDisplay(display);
}