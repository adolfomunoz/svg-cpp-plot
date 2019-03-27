#pragma once 

#include "../core/element.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/style-attributes.h"
#include "../attributes/graphical-attributes.h"

namespace svg_cpp_plot {

class Line : public Terminal, public Attributes<Line>, public GraphicalAttributes<Line>, public StyleAttributes<Line>, public PresentationAttributes<Line> {
public:
	Line(float x1, float y1, float x2, float y2) : Terminal("line") 
	{
		set("x1",x1);
		set("y1",y1);
		set("x2",x2);
		set("y2",y2);
	}
	
	float x1() const noexcept { return get_float("x1"); }
	float x2() const noexcept { return get_float("x2"); }
	float y1() const noexcept { return get_float("y1"); }
	float y2() const noexcept { return get_float("y2"); }
	
	BoundingBox bounding_box() const noexcept override { 
		return BoundingBox(std::min(x1(),x2()),std::min(y1(),y2()),std::max(x1(),x2()),std::max(y1(),y2()));
	}
	
};

}
