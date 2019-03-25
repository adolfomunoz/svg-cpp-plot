#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <list>
#include <iostream>
#include <iomanip>

#include "object.h"
#include "object-list.h"
#include "bounding-box.h"
#include "../attributes/attributes.h"

namespace svg_cpp_plot {

class NodeBase;

class Element : public AttributesBase, public Object {	
	std::string tag_;
	NodeBase* parent_;	
	friend class NodeBase;
public:
	NodeBase* parent() const noexcept { return parent_; }

	Element(const std::string& tag) : tag_(tag),parent_(nullptr) { 
		static unsigned long ids = 0;
		std::stringstream ss;
		ss << "id" << std::setw(10) << std::setfill('0') << ids++;
		this->get_or_set("id",ss.str());	
	}
	

	constexpr const std::string& tag() const noexcept {
		return tag_;
	}
	
	std::string attributes_to_string() const noexcept {
		return AttributesBase::map_to_string("=\"","\" ");
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

}
