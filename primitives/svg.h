#pragma once 

#include "../core/node.h"
#include "../attributes/aspect-ratio-attributes.h"

namespace svg_cpp_plot {

class SVG : public NodeBase, public Attributes<SVG>, public AspectRatioAttributes<SVG> {
public:
	SVG() : NodeBase("svg") {
		set("xmlns","http://www.w3.org/2000/svg");
		set("xmlns:xlink","http://www.w3.org/1999/xlink"); //inkscape compatibility with embedded pngs
		set("width","100%");
		set("height","100%");
	}
	
	SVG& viewBox(const BoundingBox& bb) {
		set("viewBox",bb);
		return (*this);
	}
	
	SVG& automatic_viewBox() {
		return viewBox(bounding_box().expand(0.1f));
	}

	BoundingBox viewBox() {
		return get_or_set("viewBox",BoundingBox());
	}
};

std::ostream& operator<<(std::ostream& os, const SVG& s) {
	os<<s.to_string();
	return os;
}	


}
