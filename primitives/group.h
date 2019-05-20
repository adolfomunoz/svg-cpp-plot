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

template<typename T,typename FCombine>
class GroupGenerator : public NodeGenerator<T,FCombine>, 
	public Attributes<GroupGenerator<T,FCombine>>, public GraphicalAttributes<GroupGenerator<T,FCombine>>, 
	public StyleAttributes<GroupGenerator<T,FCombine>>, public PresentationAttributes<GroupGenerator<T,FCombine>> {
public:
	GroupGenerator(const T& t = T(), const FCombine& combine = FCombine()) : NodeGenerator<T,FCombine>("g",t,combine) { }
		
};

}
