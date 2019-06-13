#pragma once 

#include "../core/node.h"
#include "../core/node-generator.h"

namespace svg_cpp_plot {

class Defs : public NodeBase {
public:
	Defs() : NodeBase("defs") {}
};

template<typename T,typename FCombine>
class DefsGenerator : public NodeGenerator<T,FCombine> {
public:
	DefsGenerator(const T& t = T(), const FCombine& combine = FCombine()) : NodeGenerator<T,FCombine>("defs",t,combine) { }
};


}
