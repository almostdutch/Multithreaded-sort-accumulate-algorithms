/*
 * threads_guard.h
 *
 * Exception safe guard for vector<std::thread>, vector<interruptible_thread>
 *
 */

#include <vector>
#include <thread>
#include "interruptible_thread.h"

#ifndef THREADS_GUARD_H_
#define THREADS_GUARD_H_

template<typename ThreadType>
class ThreadGuard {
public:
	ThreadGuard(ThreadType &_t);
	~ThreadGuard();
private:
	ThreadType &thread;
};

template<typename ThreadType>
ThreadGuard<ThreadType>::ThreadGuard(ThreadType &_t) :
		thread(_t) {
}

template<typename ThreadType>
ThreadGuard<ThreadType>::~ThreadGuard() {
	if (thread.joinable())
		thread.join();
}

template<typename ThreadType>
class ThreadsGuard {
public:
	ThreadsGuard(std::vector<ThreadType> &_t);
	~ThreadsGuard();
private:
	std::vector<ThreadType> &threads;
};

template<typename ThreadType>
ThreadsGuard<ThreadType>::ThreadsGuard(std::vector<ThreadType> &_t) :
		threads(_t) {
}

template<typename ThreadType>
ThreadsGuard<ThreadType>::~ThreadsGuard() {
	for (auto &el : threads) {
		if (el.joinable())
			el.join();
	}
}

#endif /* THREADS_GUARD_H_ */
