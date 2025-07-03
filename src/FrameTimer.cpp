#include "FrameTimer.hpp"

FrameTimer& FrameTimer::getInstance() {
	static FrameTimer instance;
	return instance;
}

float FrameTimer::getDeltaTime() const {
	return this->deltaTime;
}

void FrameTimer::update() {
	const auto currentTime = std::chrono::high_resolution_clock::now();
	if (lastTime.time_since_epoch().count() == 0) {
		lastTime = currentTime; // Initialize lastTime on the first call
		return;
	}
	deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
	lastTime = currentTime;
}