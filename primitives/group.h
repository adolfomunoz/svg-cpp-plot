#pragma once 

#include "../core/node.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/style-attributes.h"
#include "../attributes/graphical-attributes.h"
#include "../core/node-generator.h"

namespace svg_cpp_plot {

class Group : public NodeBase, public Attributes<Group>, public GraphicalAttributes<Group>, public StyleAttributes<Group>, public PresentationAttributes<Group> {
public:
	Group() : NodeBase("g") {}
};

template<typename T>
class GroupGenerator : public NodeGenerator<T>, public Attributes<GroupGenerator<T>>, public GraphicalAttributes<GroupGenerator<T>>, public StyleAttributes<GroupGenerator<T>>, public PresentationAttributes<GroupGenerator<T>> {
public:
	GroupGenerator(const T& t = T()) : NodeGenerator<T>("g",t) { }
		
};

}
