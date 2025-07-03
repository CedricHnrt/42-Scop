#ifndef FRAMETIMER_HPP
#define FRAMETIMER_HPP

#include <chrono>

class FrameTimer {
	public:
		static FrameTimer& getInstance();
		FrameTimer(const FrameTimer&) = delete;
		FrameTimer& operator=(const FrameTimer&) = delete;
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
		float getDeltaTime() const;
		void update();

	private:
		std::chrono::high_resolution_clock::time_point lastTime;
		float deltaTime = 0.0f;
		FrameTimer() = default;
		~FrameTimer() = default;
};

#endif //FRAMETIMER_HPP
