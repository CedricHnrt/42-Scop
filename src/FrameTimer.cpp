#include "FrameTimer.hpp"

FrameTimer& FrameTimer::getInstance() {
	static FrameTimer instance;
	return instance;
}

float FrameTimer::getDeltaTime() const {
	return this->deltaTime;
}

void FrameTimer::printFPS() {
	std::string color;
	if (this->frameCount < 15) {
		color = RED;
	} else if (this->frameCount < 30) {
		color = YELLOW;
	} else {
		color = GREEN;
	}
	std::cout << color << "\rFPS: " << this->frameCount;
	std::cout << " " << RESET << std::flush; // Clear the line after printing FPS
	this->frameCount = 0;
	this->accumulatedTime = 0.0f;
}

void FrameTimer::limitFPS() const {
	const auto actualTime = std::chrono::high_resolution_clock::now();
	const auto targetTime = this->lastTime + std::chrono::milliseconds(static_cast<int>(1000.0f / FPS_LIMIT));
	if (actualTime < targetTime) {
		std::this_thread::sleep_for(targetTime - actualTime);
	}
}

void FrameTimer::update() {
	this->limitFPS();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	if (this->lastTime.time_since_epoch().count() == 0) {
		this->lastTime = currentTime; // Initialize lastTime on the first call
		return;
	}
	this->deltaTime = std::chrono::duration<float>(currentTime - this->lastTime).count();
	this->lastTime = currentTime;
	this->accumulatedTime += this->deltaTime;
	if (this->accumulatedTime >= 1.0f) {
		this->printFPS();
	}
	this->frameCount++;
}