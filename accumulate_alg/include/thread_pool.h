/*
 * thread_pool.h
 *
 * Interruptible thread pool (workers can steal tasks from the main thread and from each other)
 *
 */

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <memory>
#include <utility>
#include <vector>
#include <future>
#include <type_traits>
#include <exception>
#include <thread>
#include <atomic>
#include "function_wrapper.h"
#include "threads_guard.h"
#include "threadsafe_queue1.h"
#include "threadsafe_queue2.h"
#include "threadsafe_queue3.h"
#include "threadsafe_queue4.h"
#include "threadsafe_stack1.h"
#include "threadsafe_stack2.h"
#include "threadsafe_stack3.h"

template<typename T>
using ThreadSafeContainerType = ThreadSafeQueue1<T>;

class thread_pool {
private:
	typedef function_wrapper task_type;
	typedef std::unique_ptr<task_type> task_type_ptr;

	class atomic_wrapper {
	public:
		atomic_wrapper() :
				flag(false) {
		}
		~atomic_wrapper() = default;
		void set_flag() {
			flag.store(true, std::memory_order_relaxed);
		}
		void clear_flag() {
			flag.store(false, std::memory_order_relaxed);
		}
		bool status_flag() {
			return flag.load(std::memory_order_relaxed);
		}
	private:
		std::atomic<bool> flag;
	};

	class thread_interrupted: public std::exception {
	public:
		const char* what(void) const noexcept (true) override {
			return "thread interrupted";
		}
	};

	void interruption_point();
	void interrupt_worker(size_t index) {
		flags[index].set_flag();
	}
	void worker_thread(size_t index);
	task_type_ptr pop_task_from_local_stack() {
		return workers_containers[worker_index]->tryPop();
	}
	task_type_ptr pop_task_from_pool_stack() {
		return master_container.tryPop();
	}
	task_type_ptr pop_task_from_other_thread_stack();
public:
	thread_pool(size_t _Nthreads = 1);
	~thread_pool();
	thread_pool(const thread_pool&) = delete;
	thread_pool& operator=(const thread_pool&) = delete;
	thread_pool(thread_pool&&) = delete;
	thread_pool& operator=(thread_pool&&) = delete;

	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>::type> submit(
			FunctionType f);
	void run_pending_task();
	void kill_worker(size_t index);
private:
	ThreadSafeContainerType<task_type> master_container;
	std::vector<std::unique_ptr<ThreadSafeContainerType<task_type>>> workers_containers;
	static thread_local size_t worker_index;
	std::vector<atomic_wrapper> flags;
	std::vector<std::thread> threads;
	ThreadsGuard<std::thread> threads_guard;
};
thread_local size_t thread_pool::worker_index = 111; // index of main thread

thread_pool::thread_pool(size_t _Nthreads) :
		flags(_Nthreads), threads_guard(threads) {
	// max number of hardware threads
	const size_t NthreadsMax = std::thread::hardware_concurrency();
	if (_Nthreads > NthreadsMax)
		_Nthreads = NthreadsMax;

	workers_containers.reserve(_Nthreads);
	threads.reserve(_Nthreads);
	try {
		for (size_t i = 0; i < _Nthreads; ++i) {
			workers_containers.emplace_back(
					new ThreadSafeContainerType<task_type>());
			threads.emplace_back(&thread_pool::worker_thread, this, i);
		}
	} catch (...) {
		// swallow this exception
	}
}

thread_pool::~thread_pool() {
	for (size_t i = 0; i < threads.size(); ++i)
		kill_worker(i);
}

void thread_pool::interruption_point() {
	if (flags[worker_index].status_flag())
		throw thread_interrupted();
}

void thread_pool::worker_thread(size_t index) {
	worker_index = index;
	while (true) {
		try {
			interruption_point();
			run_pending_task();
		} catch (const thread_interrupted&) {
			break; // stop the worker
		}
	}
}

thread_pool::task_type_ptr thread_pool::pop_task_from_other_thread_stack() {
	for (size_t i = 0; i < workers_containers.size(); ++i) {
		const size_t index = (worker_index + i + 1) % workers_containers.size();
		if (task_type_ptr task = workers_containers[index]->tryPop())
			return task;
	}
	return task_type_ptr();
}

void thread_pool::kill_worker(size_t index) {
	interrupt_worker(index);
	if (threads[index].joinable())
		threads[index].join();
}

template<typename FunctionType>
std::future<typename std::result_of<FunctionType()>::type> thread_pool::submit(
		FunctionType f) {
	typedef typename std::result_of<FunctionType()>::type result_type;
	std::packaged_task<result_type()> task(std::move(f));
	std::future<result_type> res(task.get_future());
	if (worker_index == 111)
		master_container.push(std::move(task));
	else
		workers_containers[worker_index]->push(std::move(task));
	return res;
}

void thread_pool::run_pending_task() {
	if (worker_index == 111) {
		if (task_type_ptr task = pop_task_from_pool_stack())
			return task->operator()();
		else
			std::this_thread::yield();
	} else {
		if (task_type_ptr task = pop_task_from_local_stack())
			return task->operator()();
		else if (task_type_ptr task = pop_task_from_pool_stack())
			return task->operator()();
		else if (task_type_ptr task = pop_task_from_other_thread_stack())
			return task->operator()();
		else
			std::this_thread::yield();
	}
}

#endif /* THREAD_POOL_H_ */

