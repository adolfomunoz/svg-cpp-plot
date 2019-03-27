#pragma once 

#include "../core/element.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/style-attributes.h"
#include "../attributes/graphical-attributes.h"
#include <map>

namespace svg_cpp_plot {

class GroupZOrdered : public NotTerminal, public Attributes<GroupZOrdered>, public GraphicalAttributes<GroupZOrdered>, public StyleAttributes<GroupZOrdered>, public PresentationAttributes<GroupZOrdered> {
	std::multimap<float, std::shared_ptr<Element>> children;
	
private:
	template<typename T> 
	T& set_z(T& t, float z) { 
		t.set_custom("z",z);
		return t; 
	}

public:
	GroupZOrdered() : NotTerminal("g") {}
	
	std::string content() const noexcept override {
		std::stringstream sstr;
		for (auto o : children) if (o.second) sstr<<"   "<<o.second->to_string()<<std::endl;
		return sstr.str();
	}

	//Add directly the pointer (instancing?)
	Object& add(const std::shared_ptr<Element>& o, float z) {
		return *((*children.insert(std::pair(-z,o))).second);
	}	
	
	//Add by copy
	template<typename T> 
	T& add(const T& t, float z) {
		return set_z(static_cast<T&>(*((*children.insert(std::pair(-z, std::make_shared<std::decay_t<T>>(t)))).second)),z);
	}

	//Add by move if possible
	template<typename T> 
	T& add(T&& t, float z) {
		return set_z(static_cast<T&>(*((*children.insert(std::pair(-z, std::make_shared<std::decay_t<T>>(std::forward<T>(t))))).second)),z);
	}
	
	BoundingBox bounding_box() const noexcept override { 
		BoundingBox bb;
		for (auto o : children) if (o.second) bb.join(o.second->bounding_box());
		bb.expand(get_float("stroke-width",0.0f));
		return bb;
	}
};


}
