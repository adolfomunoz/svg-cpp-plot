#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <list>
#include <limits>

namespace svg_cpp_plot {

class BoundingBox {
	std::tuple<float, float> min, max;
public:
	BoundingBox(float xmin, float ymin, float xmax, float ymax) :
		min(xmin,ymin), max(xmax,ymax) { }
		
	BoundingBox() : BoundingBox(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min()) {}
		
	void join(const BoundingBox& that) { 
		if (std::get<0>(that.min)<std::get<0>(min)) std::get<0>(min)=std::get<0>(that.min);
		if (std::get<1>(that.min)<std::get<1>(min)) std::get<1>(min)=std::get<1>(that.min);
		if (std::get<0>(that.max)>std::get<0>(max)) std::get<0>(max)=std::get<0>(that.max);
		if (std::get<1>(that.max)>std::get<1>(max)) std::get<1>(max)=std::get<1>(that.max);
	}

	std::string to_string() const {
		std::stringstream sstr;
		sstr<<std::get<0>(min)<<" "<<std::get<1>(min)<<" "<<
				(std::get<0>(max) - std::get<0>(min))<<" "<<
				(std::get<1>(max) - std::get<1>(min));
		return sstr.str();
	}
};

class Object {
	std::string tag_;
	std::unordered_map<std::string, std::string> attributes;
	
public:
	Object(const std::string& tag) : tag_(tag) { }
	
	std::string& operator[](const std::string& key) noexcept { return attributes[key]; }
	
	template<typename T>
	Object& set(const std::string& key, const T& value) noexcept { 
		attributes[key] = std::to_string(value); return *this;
	}

	Object& set(const std::string& key, const char* value) noexcept { 
		attributes[key] = std::string(value); return *this;
	}

	Object& set(const std::string& key, const std::string& value) noexcept { 
		attributes[key] = value; return *this;
	}
	
	
	std::optional<std::string> get(const std::string& key) const noexcept {
		try {
			return attributes.at(key);
		} catch (const std::out_of_range& e) {
			return {};
		}
	}
	
	float get_float(const std::string& key, float default_value = 0) const noexcept {
		if (auto v = get(key)) return stof(v.value()); 
		else return default_value;
	}
	
	std::string attributes_to_string() const noexcept {
		std::stringstream sstr;
		for (const auto & [k,v] : attributes)
			sstr<<k<<"=\""<<v<<"\" ";
		return sstr.str();
	}
	
	constexpr const std::string& tag() const noexcept {
		return tag_;
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

class Node : public Object, public std::list<std::shared_ptr<Object>> {
public:
	Node(const std::string& tag) : Object(tag) {}
	
	std::string to_string() const noexcept override {
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
	
	BoundingBox bounding_box() const noexcept override { 
		BoundingBox bb;
		for (auto o : (*this)) if (o) bb.join(o->bounding_box());
		return bb;
	}

};

}