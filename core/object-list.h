#pragma once

#include <sstream>
#include <memory>
#include "object.h"
#include <list>
#include <iostream>

namespace svg_cpp_plot {

template<typename O = Object>
class ObjectList {
	static_assert(std::is_base_of_v<Object,O>,"Should inherit from object");
	std::list<std::shared_ptr<O>> object_list;
public:
	//Add directly the pointer (instancing?)
	O& add(const std::shared_ptr<O>& o) {
		children.push_back(o); return (*children.back());
	}

	//Add by copy
	template<typename T> 
	T& add(const T& t) {
		children.push_back(std::make_shared<std::decay_t<T>>(t));
		return static_cast<T&>(*children.back());
	}

	//Add by move if possible
	template<typename T> 
	T& add(T&& t) {
		children.push_back(std::make_shared<std::decay_t<T>>(std::forward<T>(t)));
		return static_cast<T&>(*children.back());
	}

	std::string list_to_string(const std::string& sep) const noexcept {
		std::stringstream sstr;
		for (const auto& o: object_list) sstr<<o<<sep;
		return sstr.str();
	}
};



}
