/*
 * interruptible_thread.h
 *
 * Interruptible thread for thread pool
 *
 */

#ifndef INTERRUPTIBLE_THREAD_H_
#define INTERRUPTIBLE_THREAD_H_

#include <atomic>
#include <thread>
#include <exception>
#include <utility>

class interruptible_thread {
public:
	class thread_interrupted: public std::exception {
	public:
		const char* what(void) const noexcept (true) override {
			return "thread interrupted";
		}
	};
public:
	template<typename Fn, typename ... Ts>
	interruptible_thread(Fn &&fn, Ts &&... pars);
	bool joinable() {
		return internal_thread.joinable();
	}
	void join() {
		internal_thread.join();
	}
	void detach() {
		internal_thread.detach();
	}

	void set_flag(void) {
		interrupt_flag.store(true, std::memory_order_relaxed);
	}
	void clear_flag(void) {
		interrupt_flag.store(false, std::memory_order_relaxed);
	}
	bool status_flag(void) {
		return interrupt_flag.load(std::memory_order_relaxed);
	}

	~interruptible_thread() = default;
	interruptible_thread(const interruptible_thread&) = delete;
	interruptible_thread& operator=(const interruptible_thread&) = delete;
	interruptible_thread(interruptible_thread &&rhs);
	interruptible_thread& operator=(interruptible_thread &&rhs);
private:
	std::atomic<bool> interrupt_flag;
	std::thread internal_thread;
};

template<typename Fn, typename ... Ts>
interruptible_thread::interruptible_thread(Fn &&fn, Ts &&... pars) :
		interrupt_flag(false), internal_thread(std::move(fn),
				std::forward<Ts>(pars)...) {
}

interruptible_thread::interruptible_thread(interruptible_thread &&rhs) :
		interrupt_flag(rhs.interrupt_flag.load(std::memory_order_relaxed)), internal_thread(
				std::move(rhs.internal_thread)) {
}

interruptible_thread& interruptible_thread::operator=(
		interruptible_thread &&rhs) {
	interrupt_flag.store(rhs.interrupt_flag.load(std::memory_order_relaxed),
			std::memory_order_relaxed);
	internal_thread = std::move(rhs.internal_thread);
	return *this;
}

#endif /* INTERRUPTIBLE_THREAD_H_ */
