#pragma once

#include "../core/object-list.h"

namespace svg_cpp_plot {

template<typename T, bool IsObject = std::is_base_of_v<Object,T>>
class AttributeList : public Object, public ObjectList<T> {

public:
	AttributeList() {}
	template<typename Q>
	AttributeList(const Q& t, std::enable_if_t<std::is_base_of_v<T,Q>>* test = nullptr) { this->add(t); }
	template<typename Q>
	AttributeList(Q&& t, std::enable_if_t<std::is_base_of_v<T,Q>>* test = nullptr) { this->add(std::forward<Q>(t)); }
	AttributeList(const std::shared_ptr<T>& t) { this->add(t); }

	AttributeList(const std::initializer_list<std::shared_ptr<T>>& il) {
		for (const auto& t : il) this->add(t);
	}
    
	std::string to_string() const noexcept override { return this->list_to_string(" "); }	
};

template<typename T>
class AttributeList<T,false> : public Object, public ObjectList<ObjectConstant<T>> {
public:
	T& add(const T& t) { return this->ObjectList<ObjectConstant<T>>::add(ObjectConstant<T>(t)).value(); }
	T& add(T&& t) { return this->ObjectList<ObjectConstant<T>>::add(ObjectConstant<T>(std::forward<T>(t))).value(); }

	AttributeList() {}
	AttributeList(const T& t) { this->add(t); }
	AttributeList(T&& t) { this->add(std::forward<T>(t)); }
	AttributeList(std::initializer_list<T> il) {
		for (const T& t : il) this->add(t);
	}

	std::string to_string() const noexcept override { return this->list_to_string(" "); }	
};



}

