#pragma once

#include "generator.h"
#include "node.h"
#include <list>

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
	template<typename U> 
	U& add(const U& u) {
		return static_cast<U&>(add_ptr(std::make_shared<std::decay_t<U>>(u)));
	}

	//Add by move if possible
	template<typename U> 
	U& add(U&& u) {
		return static_cast<U&>(this->add_ptr(std::make_shared<std::decay_t<U>>(std::forward<U>(u))));
	}

	template<typename P>
	void remove_if(const P& p) {
		generator_list.remove_if([&p] (const std::shared_ptr<Generator<T>>& o) { return p(*o); });
	}

	NodeGenerator(const std::string& tag, const T& t) : NotTerminal(tag), t(t) { }
	void set(const T& t) { this->t = t; }
	
	std::string content() const noexcept override {
		std::cerr<<"GROUP"<<std::endl;
		std::cerr<<generator_list.size()<<std::endl;
		std::stringstream sstr;
		for (auto g : generator_list) sstr<<g->to_string(this->t)<<std::endl;
		std::cerr<<sstr.str();
		return sstr.str();
	}
};

}
