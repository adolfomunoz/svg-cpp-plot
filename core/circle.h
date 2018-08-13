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
	
	float cx() const noexcept { return get_float("cx"); }
	float cy() const noexcept { return get_float("cy"); }
	float r() const noexcept  { return get_float("r");  }
	
	BoundingBox bounding_box() const noexcept override { 
		return BoundingBox(cx()-r(),cy()-r(),cx()+r(),cy()+r());
	}
	
};

}