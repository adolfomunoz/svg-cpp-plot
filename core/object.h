#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <list>
#include <iostream>

#include "bounding-box.h"
#include "attributes.h"

namespace svg_cpp_plot {

class Object : public AttributesBase {
	std::string tag_;
	
public:
	Object(const std::string& tag) : tag_(tag) { }
	

	constexpr const std::string& tag() const noexcept {
		return tag_;
	}
	
	std::string attributes_to_string() const noexcept {
		return AttributesBase::attributes_to_string("=\"","\" ");
	}

	virtual std::string to_string() const noexcept = 0;
	virtual BoundingBox bounding_box() const noexcept { return BoundingBox(); }
};	

class Terminal : public Object {
public:
	Terminal(const std::string& tag) : Object(tag) {}
	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"<"<<tag()<<" "<<attributes_to_string()<<"/>";
		return sstr.str();
	}
};

class NotTerminal : public Object {
public:
	NotTerminal(const std::string& tag) : Object(tag) {}
	virtual std::string content() const noexcept = 0;
	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"<"<<tag()<<" "<<attributes_to_string()<<">"<<std::endl;
		sstr<<content()<<std::endl;
		sstr<<"</"<<tag()<<">"<<std::endl;
		return sstr.str();
	}
};

class Node : public NotTerminal {
	std::list<std::shared_ptr<Object>> children;
public:
	Node(const std::string& tag) : NotTerminal(tag) {}
	
	std::string content() const noexcept override {
		std::stringstream sstr;
		for (auto o : children) if (o) sstr<<"   "<<o->to_string()<<std::endl;
		return sstr.str();
	}

	//Add directly the pointer (instancing?)
	Object& add(const std::shared_ptr<Object>& o) {
		children.push_back(o);
		return (*children.back());
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
	
	BoundingBox bounding_box() const noexcept override { 
		BoundingBox bb;
		for (auto o : children) if (o) bb.join(o->bounding_box());
		bb.expand(get_float("stroke-width",0.0f));
		return bb;
	}
};

}
