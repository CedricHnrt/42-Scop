#include "WindowManager.hpp"

static bool validateResolution(const std::vector<int>& windowRes, const std::vector<int>& maxRes) {
	if (windowRes.size() != 2)
		return false;
	if (windowRes[0] > maxRes[0] || windowRes[1] > maxRes[1])
		return false;
	if (windowRes[0] < 800 || windowRes[1] < 600)
		return false;
	return true;
}

void WindowManager::resolveName(const char *name) {
	if (name)
		this->name = name;
	this->name = "SCOP - " + ObjectData::getInstance().getFilename();
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

	Atom wmDelete = XInternAtom(this->display, "WM_DELETE_WINDOW", False); // Create a delete window atom
	XSetWMProtocols(this->display, this->window, &wmDelete, 1); // Set the delete window protocol
	this->wmDelete = wmDelete;
	
	glXMakeCurrent(this->display, this->window, context); // Make the context current
}

void WindowManager::loop() {
	XEvent event;
	this->running = true;
	while (this->running) {
		while (XPending(this->display)) {
			XNextEvent(this->display, &event);
			switch (event.type) {
				case KeyPress:
					if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_Escape)) {
						this->running = false; // Exit the loop on Escape key press
						std::cout << "Escape key pressed. Exiting animation loop." << std::endl;
					}
					break;
				case ClientMessage:
					if (event.xclient.data.l[0] == this->wmDelete) {
						this->running = false; // Exit the loop on window close request
						std::cout << "Window close requested." << std::endl;
					}
					break;
				case Expose:
					if (event.xexpose.count == 0) {
						std::cout << "Window exposed. Redrawing..." << std::endl; // Handle window exposure
						//TODO: handle window exposure, e.g., redraw the scene
					}
					break;
				default: std::cout << "Unhandled event type: " << event.type << std::endl; // Log unhandled events
					break;
			}
		}
		//TODO: Handle rendering
		this->render();
	}
}

#include <GL/glu.h>

void WindowManager::render() const {
	glEnable(GL_DEPTH_TEST); // REMOVE
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TEST
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// gluPerspective(60.0, static_cast<double>(this->resolution[0]) / this->resolution[1], 0.1, 100.0); // Set perspective projection
	//
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// gluLookAt(
	// 	0.0, 0.0, 5.0,  // position caméra
	// 	0.0, 0.0, 0.0,  // regarde vers le centre
	// 	0.0, 1.0, 0.0   // up = Y
	// );
	//
	// static float angle = 0.0f;
	// angle += 1.0f; // rotation continue
	// glRotatef(angle, 0.0f, 1.0f, 0.0f);
	
	//TEST
	
	ObjectData::getInstance().draw(); // Draw the object data
	glXSwapBuffers(this->display, this->window); // Swap buffers to display the rendered frame
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
	glXDestroyContext(this->display, glXGetCurrentContext()); // Destroy the OpenGL context
	XDestroyWindow(this->display, this->window);
	if (this->display)
		XCloseDisplay(display);
}