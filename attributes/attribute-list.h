#pragma once

#include "../core/object-list.h"

namespace svg_cpp_plot {

template<typename T, bool IsObject = std::is_base_of_v<Object,T>>
class AttributeList : public Object, public ObjectList<T> {

public:
	AttributeList(std::initializer_list<T> il) {
		for (const T& t : il) this->add(t);
	}

	std::string to_string() const noexcept override { return this->list_to_string(" "); }	
};

template<typename T>
class AttributeList<T,false> : public Object, public ObjectList<ObjectConstant<T>> {

public:
	AttributeList(std::initializer_list<T> il) {
		for (const T& t : il) this->add(ObjectConstant<T>(t));
	}

	std::string to_string() const noexcept override { return this->list_to_string(" "); }	
};



}

