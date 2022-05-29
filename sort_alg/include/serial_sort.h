/*
 * serial_sort.h
 *
 * Serial sort algorithm
 *
 */

#ifndef SERIAL_SORT_H_
#define SERIAL_SORT_H_

#include <utility>
#include <list>
#include <iterator>
#include <algorithm>

template<typename T>
std::list<T> serial_sort(std::list<T> input) {
	if (input.empty()) {
		return input;
	}

	std::list<T> result;
	result.splice(result.cbegin(), input, input.cbegin());
	const T &partition_val = *result.cbegin();

	typename std::list<T>::const_iterator divide_point = std::partition(
			input.begin(), input.end(), [&](const T &t) -> bool {
				return t < partition_val;
			});
	std::list<T> lower_part;
	lower_part.splice(lower_part.cend(), input, input.cbegin(), divide_point);

	std::list<T> new_lower(serial_sort(std::move(lower_part)));
	std::list<T> new_higher(serial_sort(std::move(input)));

	result.splice(result.cend(), new_higher);
	result.splice(result.cbegin(), new_lower);
	return result;
}

#endif /* SERIAL_SORT_H_ */
