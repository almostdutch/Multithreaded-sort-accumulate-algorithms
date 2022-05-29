/*
 * parallel_sort.h
 *
 * Multithreaded sort algorithm
 *
 */

#ifndef PARALLEL_SORT_H_
#define PARALLEL_SORT_H_

#include <list>
#include <vector>
#include <future>
#include <memory>
#include <chrono>
#include <utility>
#include <iterator>
#include <algorithm>
#include "thread_pool.h"

template<typename T>
class sorter_list {
public:
	sorter_list(size_t Nthreads) :
			pool(Nthreads - 1) {
	}

	~sorter_list() = default;

	std::list<T> do_sort(std::list<T> input) {
		if (input.empty()) {
			return input;
		}

		std::list<T> result;
		result.splice(result.begin(), input, input.begin());
		const T &partition_val = *result.begin();
		typename std::list<T>::iterator divide_point = std::partition(
				input.begin(), input.end(), [&](const T &val) {
					return val < partition_val;
				});

		std::list<T> new_lower_chunk;
		new_lower_chunk.splice(new_lower_chunk.end(), input, input.begin(),
				divide_point);
		std::future<std::list<T> > new_lower = pool.submit(
				[&]() -> std::list<T> {
					return do_sort(std::move(new_lower_chunk));
				});

		std::list<T> new_higher(do_sort(std::move(input)));
		result.splice(result.end(), new_higher);
		while (new_lower.wait_for(std::chrono::seconds(0))
				!= std::future_status::ready) {
			pool.run_pending_task();
		}

		result.splice(result.begin(), new_lower.get());
		return result;
	}
private:
	thread_pool pool;
};
template<typename T>
std::list<T> parallel_sort(std::list<T> input, size_t Nthreads = 2) {
	if (input.empty()) {
		return input;
	}

	// max number of hardware threads
	const size_t NthreadsMax = std::thread::hardware_concurrency();
	if (Nthreads > NthreadsMax)
		Nthreads = NthreadsMax;

	sorter_list<T> s(Nthreads);
	return s.do_sort(input);
}

#endif /* PARALLEL_SORT_H_ */
