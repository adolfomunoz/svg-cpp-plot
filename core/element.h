#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <list>
#include <iostream>

#include "object.h"
#include "bounding-box.h"
#include "attributes.h"

namespace svg_cpp_plot {

class Element : public AttributesBase, public Object {
	std::string tag_;
	
public:
	Element(const std::string& tag) : tag_(tag) { }
	

	constexpr const std::string& tag() const noexcept {
		return tag_;
	}
	
	std::string attributes_to_string() const noexcept {
		return AttributesBase::attributes_to_string("=\"","\" ");
	}

	virtual BoundingBox bounding_box() const noexcept { return BoundingBox(); }
};	

class Terminal : public Element {
public:
	Terminal(const std::string& tag) : Element(tag) {}
	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"<"<<tag()<<" "<<attributes_to_string()<<"/>";
		return sstr.str();
	}
};

class NotTerminal : public Element {
public:
	NotTerminal(const std::string& tag) : Element(tag) {}
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
	std::list<std::shared_ptr<Element>> children;
public:
	Node(const std::string& tag) : NotTerminal(tag) {}
	
	std::string content() const noexcept override {
		std::stringstream sstr;
		for (auto o : children) if (o) sstr<<"   "<<o->to_string()<<std::endl;
		return sstr.str();
	}

	//Add directly the pointer (instancing?)
	Element& add(const std::shared_ptr<Element>& o) {
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
