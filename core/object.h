#pragma once

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <list>

namespace svg_cpp_plot {

class Object {
	std::string tag_;
public:
	Object(const std::string& tag) : tag_(tag) { }
	
	std::map<std::string, std::string> attributes;
	
	std::string attributes_to_string() const {
		std::stringstream sstr;
		for (const auto & [k,v] : attributes)
			sstr<<k<<"=\""<<v<<"\" ";
		return sstr.str();
	}
	
	const std::string& tag() const {
		return tag_;
	}
	
	virtual std::string to_string() const = 0;

};	

class Terminal : public Object {
public:
	Terminal(const std::string& tag) : Object(tag) {}
	std::string to_string() const override {
		std::stringstream sstr;
		sstr<<"<"<<tag()<<" "<<attributes_to_string()<<"/>";
		return sstr.str();
	}
};

class Node : public Object, public std::list<std::shared_ptr<Object>> {
public:
	Node(const std::string& tag) : Object(tag) {}
	
	std::string to_string() const override {
		std::stringstream sstr;
		sstr<<"<"<<tag()<<" "<<attributes_to_string()<<">"<<std::endl;
		for (auto o : (*this)) if (o) sstr<<"   "<<o->to_string()<<std::endl;
		sstr<<"</"<<tag()<<">"<<std::endl;
		return sstr.str();
	}

	//Add directly the pointer (instancing?)
	Node& add(const std::shared_ptr<Object>& o) {
		this->push_back(o);
		return *this;
	}	
	//Add by copy
	template<typename T> 
	Node& add(const T& t) {
		this->push_back(std::make_shared<T>(t));
		return *this;
	}
	//Add by move if possible
	template<typename T> 
	Node& add(T&& t) {
		this->push_back(std::make_shared<T>(std::forward<T>(t)));
		return *this;
	}
};

}