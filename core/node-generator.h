#pragma once

#include "generator.h"

namespace svg_cpp_plot {

template<typename T>
class NodeGenerator : public NotTerminal {
protected:
	T t;
	std::list<std::shared_ptr<Generator<T>>> generator_list;
	
	Generator<T>& add_ptr(const std::shared_ptr<Generator<T>>& o) {
		generator_list.push_back(o); 
		return (*generator_list.back());
	}
public:
	//Add directly the pointer (instancing?)
	Generator<T>& add(const std::shared_ptr<Generator<T>>& o) {
		return add_ptr(o);
	}

	//Add by copy
	template<typename T> 
	T& add(const T& t) {
		return static_cast<T&>(add_ptr(std::make_shared<std::decay_t<T>>(t)));
	}

	//Add by move if possible
	template<typename T> 
	T& add(T&& t) {
		return static_cast<T&>(add_ptr(std::make_shared<std::decay_t<T>>(std::forward<T>(t))));
	}

	template<typename P>
	void remove_if(const P& p) {
		object_list.remove_if([&p] (const std::shared_ptr<Generator<T>>& o) { return p(*o); });
	}

	NodeGenerator(const std::string& tag, const T& t) : NotTerminal(tag), t(t) { }
	void set(const T& t) { this->t = t; }
	
	std::string content() const noexcept override {
		std::stringstream sstr;
		for (auto g : generator_list) sstr<<g->to_string(t)<<std::endl;
	}
};

}
