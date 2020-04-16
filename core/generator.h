#pragma once

#include <string>
#include "object.h"

namespace svg_cpp_plot {

template<typename T>
class Generator {
public:
	virtual std::string to_string(const T& t) const noexcept = 0;
};

template<typename T, typename O>
class ObjectGenerator : public Generator<T> {
	static_assert(!std::is_abstract_v<O>,"Object type should not be abstract");
	static_assert(std::is_base_of_v<Object,O>,"Object type should inherit from Object");
public:
	virtual O object(const T& t) const noexcept = 0;
	std::string to_string(const T& t) const noexcept override {
		return this->object(t).to_string();
	}
};


}
