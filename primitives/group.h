#pragma once 

#include "../core/node.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/style-attributes.h"

namespace svg_cpp_plot {

class Group : public NodeBase, public Attributes<Group>, public StyleAttributes<Group>, public PresentationAttributes<Group> {
public:
	Group() : NodeBase("g") {}
};

}
