#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <list>
#include <limits>

namespace svg_cpp_plot {

template<typename T>
struct is_2d_point {
	static constexpr bool value = std::tuple_size_v<T> == 2 &&
			std::is_floating_point_v<std::tuple_element_t<0,T>> &&
			std::is_floating_point_v<std::tuple_element_t<1,T>>;
};

template<typename T>
constexpr bool is_2d_point_v = is_2d_point<T>::value;


class BoundingBox {
	std::tuple<float, float> min, max;
public:
	BoundingBox(float xmin, float ymin, float xmax, float ymax) :
		min(xmin,ymin), max(xmax,ymax) { }

	BoundingBox(const std::tuple<float,float>& min, const std::tuple<float, float>& max) :
		min(min), max(max) { }
		
	BoundingBox() : BoundingBox(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min()) {}
		
	BoundingBox& join(const BoundingBox& that) { 
		if (std::get<0>(that.min)<std::get<0>(min)) std::get<0>(min)=std::get<0>(that.min);
		if (std::get<1>(that.min)<std::get<1>(min)) std::get<1>(min)=std::get<1>(that.min);
		if (std::get<0>(that.max)>std::get<0>(max)) std::get<0>(max)=std::get<0>(that.max);
		if (std::get<1>(that.max)>std::get<1>(max)) std::get<1>(max)=std::get<1>(that.max);
		return (*this);
	}

	//A point
	BoundingBox& join(const std::tuple<float, float>& p) {
		return join(BoundingBox(p,p));
	}

	//Expand by a distance in x and y
	BoundingBox& expand(float dx, float dy) {
		std::get<0>(min)-=dx;
		std::get<1>(min)-=dy;
		std::get<0>(max)+=dx;
		std::get<1>(max)+=dy;
		return(*this);
	}
	//Expand by a distance
	BoundingBox& expand(float f) {
		return expand(f,f);
	}
	
	//Expand by a relative percentage
	BoundingBox& expand_rate(float rate) {
		return expand(rate*(std::get<0>(max)-std::get<0>(min)),
					rate*(std::get<1>(max)-std::get<1>(min)));
	}

	std::string to_string() const {
		std::stringstream sstr;
		sstr<<std::get<0>(min)<<" "<<std::get<1>(min)<<" "<<
				(std::get<0>(max) - std::get<0>(min))<<" "<<
				(std::get<1>(max) - std::get<1>(min));
		return sstr.str();
	}
};

class AttributesBase {
	std::unordered_map<std::string, std::string> attributes;
public:
	std::string& operator[](const std::string& key) noexcept { return attributes[key]; }
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
	float get_int(const std::string& key, int default_value = 0) const noexcept {
		if (auto v = get(key)) return stoi(v.value()); 
		else return default_value;
	}

	std::string attributes_to_string(const std::string& middle, 
			                 const std::string& end) const noexcept {
		std::stringstream sstr;
		for (const auto & [k,v] : attributes)
			sstr<<k<<middle<<v<<end;
		return sstr.str();
	}
};

//CRTP
template<typename T>
class Attributes {
public:
	template<typename V>
	T& set(const std::string& key, const V& value) noexcept { 
		static_cast<T&>(*this)[key] = std::to_string(value); return static_cast<T&>(*this);
	}

	T& set(const std::string& key, const char* value) noexcept { 
		static_cast<T&>(*this)[key] = std::string(value); return static_cast<T&>(*this);
	}

	T& set(const std::string& key, const std::string& value) noexcept { 
		static_cast<T&>(*this)[key] = value; return static_cast<T&>(*this);
	}
	
	template<typename V>
	T& set_custom(const std::string& key, const V& value) noexcept { 
		static_cast<T&>(*this)[key] = std::string("data-")+std::to_string(value); 
		return static_cast<T&>(*this);
	}

	T& set_custom(const std::string& key, const char* value) noexcept { 
		static_cast<T&>(*this)[key] = std::string("data-")+std::string(value); return static_cast<T&>(*this);
	}

	T& set_custom(const std::string& key, const std::string& value) noexcept { 
		static_cast<T&>(*this)[key] = std::string("data-")+value; return static_cast<T&>(*this);
	}
	
};

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

class Node : public Object {
	std::list<std::shared_ptr<Object>> children;
public:
	Node(const std::string& tag) : Object(tag) {}
	
	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"<"<<tag()<<" "<<attributes_to_string()<<">"<<std::endl;
		for (auto o : children) if (o) sstr<<"   "<<o->to_string()<<std::endl;
		sstr<<"</"<<tag()<<">"<<std::endl;
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
