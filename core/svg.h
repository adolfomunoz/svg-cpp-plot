#pragma once 

#include "element.h"

namespace svg_cpp_plot {

class SVG : public Node, public Attributes<SVG> {
public:
	SVG() : Node("svg") {
		set("xmlns","http://www.w3.org/2000/svg");
		set("width","100%");
		set("height","100%");
	}
	
	SVG& set_viewBox(const BoundingBox& bb) {
		set("viewBox",bb.to_string());
		return (*this);
	}
	
	SVG& automatic_viewBox() {
		return set_viewBox(bounding_box().expand(0.1f));
	}
};

std::ostream& operator<<(std::ostream& os, const SVG& s) {
	os<<s.to_string();
	return os;
}	


}
