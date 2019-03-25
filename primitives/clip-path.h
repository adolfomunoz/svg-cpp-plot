#pragma once 

#include "../core/node.h"

namespace svg_cpp_plot {

class ClipPath : public NodeBase, public Attributes<ClipPath> {
public:
	ClipPath() : NodeBase("clipPath") {}
};

}
