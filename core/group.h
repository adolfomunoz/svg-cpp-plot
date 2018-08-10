#pragma once 

#include "object.h"

namespace svg_cpp_plot {

class Group : public Node {
public:
	Group() : Node("g") {}
};

}