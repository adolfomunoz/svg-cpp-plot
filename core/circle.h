#pragma once 

#include "object.h"

namespace svg_cpp_plot {

class Circle : public Terminal {
public:
	Circle(float cx, float cy, float r) : Terminal("circle") 
	{
		set("cx",cx);
		set("cy",cy);
		set("r",r);
	}
	
	float cx() const { return std::stof(get("cx").value_or("0")); }
	float cy() const { return std::stof(get("cy").value_or("0")); }
	float r() const { return std::stof(get("r").value_or("0")); }
	
	BoundingBox bounding_box() const noexcept override { 
		return BoundingBox(cx()-r(),cy()-r(),cx()+r(),cy()+r());
	}
	
};

}