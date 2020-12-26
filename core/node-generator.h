#pragma once

#include "element-generator.h"
#include "node.h"
#include <list>

namespace svg_cpp_plot {

template<typename T, typename FCombine>
class NodeGenerator : public NotTerminal, public Generator<T> {
protected:
	T t;
	FCombine combine;
	std::list<std::shared_ptr<Generator<T>>> generator_list;

public:	//This is public because sometimes it is not propperly identified as a pointer and it misses the call below
	Generator<T>& add_ptr(const std::shared_ptr<Generator<T>>& o) {
		generator_list.push_back(o);
		return (*generator_list.back());
	}
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

	NodeGenerator(const std::string& tag, const T& t = T(), const FCombine& combine = FCombine()) : NotTerminal(tag), t(t), combine(combine) { }
	void set_parameter(const T& t) { this->t = t; }
    const T& parameter() const { return t; }
	
	std::string to_string(const T& t) const noexcept override {
		std::stringstream sstr;
		sstr<<"<"<<tag()<<" "<<attributes_to_string()<<">"<<std::endl;
		for (auto g : generator_list) sstr<<g->to_string(combine(t,this->t))<<std::endl;
		sstr<<"</"<<tag()<<">"<<std::endl;
		return sstr.str();
	}
	
	std::string content() const noexcept override {
		std::stringstream sstr;
		for (auto g : generator_list) sstr<<g->to_string(this->t)<<std::endl;
		return sstr.str();
	}

};

}
