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
		throw RuntimeException("Failed to open X display");
	}
	this->screen = DefaultScreen(this->display); // Get the default screen

	this->resolveName(name); // Set Window Name
	this->resolveResolution(windowRes); // Set Window Resolution
	
	int visualAttribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None}; // Attributes for the visual
	XVisualInfo* visual = glXChooseVisual(this->display, this->screen, visualAttribs); // Choose a visual that supports OpenGL
	if (!visual) {
		throw RuntimeException("Failed to get a visual from X screen");
	}
	const Window root = RootWindow(this->display, this->screen); // Get the root window of the screen
	Colormap colormap = XCreateColormap(this->display, root, visual->visual, AllocNone); // Create a colormap for the visual
	XSetWindowAttributes attributes;
	attributes.colormap = colormap;
	attributes.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask; // Set the event mask for the window
	
	this->window = XCreateWindow(this->display, root, 0, 0, this->resolution[0], this->resolution[1], 0,
			visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask, &attributes); // Create the window

	XStoreName(this->display, this->window, this->name.c_str()); // Set the window name
	XMapWindow(this->display, this->window); // Map the window to the screen
	GLXContext context = glXCreateContext(this->display, visual, nullptr, GL_TRUE);
	if (!context) {
		throw RuntimeException("Failed to create OpenGL context");
	}

	Atom wmDelete = XInternAtom(this->display, "WM_DELETE_WINDOW", False); // Create a delete window atom
	XSetWMProtocols(this->display, this->window, &wmDelete, 1); // Set the delete window protocol
	this->wmDelete = wmDelete;
	
	glXMakeCurrent(this->display, this->window, context); // Make the context current
}

void WindowManager::updateProjectionMatrix() {
	glMatrixMode(GL_PROJECTION);
	this->projectionMatrix = Mat4::perspective(60.0f,
		static_cast<float>(this->resolution[0]) / static_cast<float>(this->resolution[1]),
		0.1f, 100.0f);
	glLoadIdentity(); // Reset the projection matrix
	glLoadMatrixf(this->projectionMatrix.data());
}

void WindowManager::keyHandler(const XEvent& event) {
	if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_Escape)) {
		this->running = false;
	} else if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_w)) {
		ObjectData::getInstance().moveObject(BACKWARD);
	} else if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_s)) {
		ObjectData::getInstance().moveObject(FORWARD);
	} else if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_a)) {
		ObjectData::getInstance().moveObject(LEFT);
	} else if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_d)) {
		ObjectData::getInstance().moveObject(RIGHT);
	} else if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_q)) {
		ObjectData::getInstance().moveObject(UP);
	} else if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_e)) {
		ObjectData::getInstance().moveObject(DOWN);
	} else if (event.xkey.keycode == XKeysymToKeycode(this->display, XK_r)) {
		ObjectData::getInstance().moveObject(CENTER);
	}
}

void WindowManager::loop() {
	XEvent event;
	this->running = true;
	while (this->running) {
		while (XPending(this->display)) {
			XNextEvent(this->display, &event);
			switch (event.type) {
				case KeyPress:
					this->keyHandler(event);
					break;
				case ClientMessage:
					if (event.xclient.data.l[0] == this->wmDelete) {
						this->running = false;
					}
					break;
				case Expose:
					if (event.xexpose.count == 0) {
						this->updateProjectionMatrix();
					}
					break;
				case ConfigureNotify:
					this->resolution[0] = event.xconfigure.width;
					this->resolution[1] = event.xconfigure.height;
					glViewport(0, 0, this->resolution[0], this->resolution[1]);
					this->updateProjectionMatrix();
					break;
				default: break;
			}
		}
		this->render();
	}
}

void WindowManager::render() {
	FrameTimer::getInstance().update();
	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	this->viewMatrix = Mat4::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f),
		Vec3(0.0f, 1.0f, 0.0f));
	this->rotationAngle += 1.00f * FrameTimer::getInstance().getDeltaTime(); // Increment rotation angle based on delta time
	this->modelMatrix = Mat4::translate(ObjectData::getInstance().getPosition()) * Mat4::rotateY(this->rotationAngle);
	glLoadIdentity();
	glLoadMatrixf((this->viewMatrix * this->modelMatrix).data()); // Load the combined projection and view matrix
	ObjectData::getInstance().draw();
	glXSwapBuffers(this->display, this->window); // Swap buffers to display the rendered frame
	
	//TODO: set texture on keyboard input
	//TODO: OPTIONAL: handle mouse input for model scaling
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