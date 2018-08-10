#pragma once 

#include "object.h"

namespace svg_cpp_plot {

class SVG : public Node {
public:
	SVG() : Node("svg") {
		attributes["xmlns"]="http://www.w3.org/2000/svg";
		
	}
};

std::ostream& operator<<(std::ostream& os, const SVG& s) {
	os<<s.to_string();
	return os;
}	


}