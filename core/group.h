#pragma once 

#include "element.h"
#include "presentation-attributes.h"
#include "style-attributes.h"

namespace svg_cpp_plot {

class Group : public Node, public Attributes<Group>, public StyleAttributes<Group>, public PresentationAttributes<Group> {
public:
	Group() : Node("g") {}
};

}
