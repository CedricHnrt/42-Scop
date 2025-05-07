#include "WindowManager.hpp"

static std::string getWindowName(const char *name) {
	if (name)
		return name;
	return "SCOP - " + ObjectData::getInstance().getFilename();
}

void WindowManager::createWindow(const char *name) {
	std::string windowName = getWindowName(name);

	Display* display = XOpenDisplay(nullptr);
	(void)display;
}

WindowManager& WindowManager::getInstance() {
	static WindowManager instance;
	return instance;
}

WindowManager::~WindowManager() {
	// Cleanup code if needed
}