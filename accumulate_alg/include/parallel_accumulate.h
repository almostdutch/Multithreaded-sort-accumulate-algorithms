/*
 * parallel_accumulate.h
 *
 * Multithreaded accumulate algorithm
 *
 */

#ifndef PARALLEL_ACCUMULATE_H_
#define PARALLEL_ACCUMULATE_H_
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <future>
#include <chrono>
#include "thread_pool.h"

template<typename Iterator, typename T>
T accumulate_chunk(Iterator first, Iterator last) {
	return std::accumulate(first, last, T { });
}

template<typename Iterator, typename T>
T parallel_accumulate(Iterator begin, Iterator end, T init,
		size_t Nthreads = 2) {

	// number of elements
	size_t Nelements = std::distance(begin, end);

	if (!Nelements)
		return init;

	// max number of hardware threads
	const size_t NthreadsMax = std::thread::hardware_concurrency();
	if (Nthreads > NthreadsMax)
		Nthreads = NthreadsMax;

	// start thread pool
	thread_pool pool(Nthreads - 1);

	// number of elements per chunk
	size_t Nel_per_chunk = std::ceil((double) Nelements / Nthreads);

	// vector of futures
	std::vector<std::future<T>> future_results(Nthreads - 1);

	// all chunks except the last one
	Iterator start = begin;
	for (size_t chunkNo = 0; chunkNo < Nthreads - 1; ++chunkNo) {
		Iterator stop = start;
		std::advance(stop, Nel_per_chunk);
		future_results[chunkNo] = pool.submit([start, stop]() -> T {
			return accumulate_chunk<Iterator, T>(start, stop);
		});
		start = stop;
	}

	// final chunk
	T result = accumulate_chunk<Iterator, T>(start, end);
	result += init;

	// sum over chunks
	for (size_t chunkNo = 0; chunkNo < Nthreads - 1; ++chunkNo) {
		result += future_results[chunkNo].get();
	}
	return result;
}

#endif /* PARALLEL_ACCUMULATE_H_ */
