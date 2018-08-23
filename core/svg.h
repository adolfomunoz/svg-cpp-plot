#pragma once 

#include "object.h"

namespace svg_cpp_plot {

class SVG : public Node, public Attributes<SVG> {
public:
	SVG() : Node("svg") {
		set("xmlns","http://www.w3.org/2000/svg");
		set("width","100%");
		set("height","100%");
	}
	
	SVG& automatic_viewBox() {
		set("viewBox",bounding_box().to_string());
		return (*this);
	}
};

std::ostream& operator<<(std::ostream& os, const SVG& s) {
	os<<s.to_string();
	return os;
}	


}
