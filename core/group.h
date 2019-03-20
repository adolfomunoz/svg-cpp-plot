#pragma once 

#include "node.h"
#include "presentation-attributes.h"
#include "style-attributes.h"

namespace svg_cpp_plot {

class Group : public NodeBase, public Attributes<Group>, public StyleAttributes<Group>, public PresentationAttributes<Group> {
public:
	Group() : NodeBase("g") {}
};

}
