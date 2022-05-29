/*
 * function_wrapper.h
 *
 * Function wrapper for thread pool
 *
 */

#ifndef FUNCTION_WRAPPER_H_
#define FUNCTION_WRAPPER_H_

#include <memory> // std::unique_ptr
#include <utility> // std::move

class function_wrapper {
private:
	// polymorphic base class
	struct impl_base {
		virtual void call()=0;
		virtual ~impl_base() {
		}
	};

	// generalized derived class
	template<typename FunctionType>
	struct impl_type: impl_base {
		impl_type(FunctionType &&f_) :
				f(std::move(f_)) {
		}
		void call() override {
			f();
		}
		FunctionType f;
	};
public:
	template<typename FunctionType>
	function_wrapper(FunctionType &&f) :
			impl(new impl_type<FunctionType>(std::move(f))) {
	}
	void operator()() {
		impl->call();
	}
	function_wrapper() = default;
	~function_wrapper() = default;
	function_wrapper(const function_wrapper&) = delete;
	function_wrapper& operator=(const function_wrapper&) = delete;

	function_wrapper(function_wrapper &&rhs) noexcept (true) :
			impl(std::move(rhs.impl)) {
	}
	function_wrapper& operator=(function_wrapper &&rhs) noexcept (true) {
		impl = std::move(rhs.impl);
		return *this;
	}
private:
	std::unique_ptr<impl_base> impl;
};

#endif /* FUNCTION_WRAPPER_H_ */
