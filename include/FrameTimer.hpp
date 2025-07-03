#ifndef FRAMETIMER_HPP
#define FRAMETIMER_HPP

#include <chrono>
#include <thread>
#include <iostream>
#include "ansiCodes.hpp"

#define FPS_LIMIT 60.0f

class FrameTimer {
	public:
		static FrameTimer& getInstance();
		FrameTimer(const FrameTimer&) = delete;
		FrameTimer& operator=(const FrameTimer&) = delete;
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
		[[nodiscard]] float getDeltaTime() const;
		void update();

	private:
		std::chrono::high_resolution_clock::time_point lastTime;
		float deltaTime = 0.0f;
		float accumulatedTime = 0.0f;
		int frameCount = 0;
		void printFPS();
		void limitFPS() const;
		FrameTimer() = default;
		~FrameTimer() = default;
};

#endif //FRAMETIMER_HPP
