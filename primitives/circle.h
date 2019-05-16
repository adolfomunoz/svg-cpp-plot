#pragma once 

#include "../core/element.h"
#include "point.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/style-attributes.h"
#include "../attributes/graphical-attributes.h"

namespace svg_cpp_plot {

class Circle : public Terminal, public Attributes<Circle>, public GraphicalAttributes<Circle>, public StyleAttributes<Circle>, public PresentationAttributes<Circle> {
public:
	Circle(float cx, float cy, float r) : Terminal("circle") 
	{
		set("cx",cx);
		set("cy",cy);
		set("r",r);
	}

	template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
	Circle(const P& p, float r) : Circle(std::get<0>(p), std::get<1>(p), r) { }
	
	float cx() const noexcept { return get_float("cx"); }
	float cy() const noexcept { return get_float("cy"); }
	float r() const noexcept  { return get_float("r");  }
	
	BoundingBox bounding_box() const noexcept override { 
		return BoundingBox(cx()-r(),cy()-r(),cx()+r(),cy()+r());
	}
	
};

}
