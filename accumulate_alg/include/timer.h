/*
 * timer.h
 *
 * Timer
 *
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

class Timer {
public:
	Timer() :
			dur(0) {
	}
	~Timer() {
	}
	void start() {
		t1 = std::chrono::high_resolution_clock::now();
	}
	void stop() {
		t2 = std::chrono::high_resolution_clock::now();
	}
	const size_t duration() {
		dur =
				std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		return dur;
	}
private:
	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;
	size_t dur;
};

#endif /* TIMER_H_ */

