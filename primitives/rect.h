#pragma once 

#include "../core/element.h"
#include "point.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/style-attributes.h"
#include "../attributes/geometry-attributes.h"
#include "../attributes/graphical-attributes.h"

namespace svg_cpp_plot {

class Rect : public Terminal, public Attributes<Rect>, public GraphicalAttributes<Rect>, public StyleAttributes<Rect>, public PresentationAttributes<Rect>, public GeometryAttributes<Rect> {
public:
    Rect() : Terminal("rect") {}

	Rect(float x1, float y1, float x2, float y2) : Terminal("rect")  {
		rect(x1,y1,x2,y2);
	}

	template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
	Rect(const P& p1, const P& p2) : Terminal("rect") { 
		rect(p1,p2);
	}
	
	BoundingBox bounding_box() const noexcept override { 
		return BoundingBox(x(),y(),x()+width(),y()+width());
	}
	
};

}
