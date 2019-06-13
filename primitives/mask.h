#pragma once 

#include "../core/node.h"
#include "../core/node-generator.h"

namespace svg_cpp_plot {

class ClipPath : public NodeBase, public Attributes<ClipPath> {
public:
	ClipPath() : NodeBase("clipPath") {}
};

template<typename T,typename FCombine>
class ClipPathGenerator : public NodeGenerator<T,FCombine>, public Attributes<ClipPathGenerator<T,FCombine>> {
public:
	ClipPathGenerator(const T& t = T(), const FCombine& combine = FCombine()) : NodeGenerator<T,FCombine>("clipPath",t,combine) { }		
};


}
