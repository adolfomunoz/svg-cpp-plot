#pragma once 

#include "../core/node.h"
#include "../core/node-generator.h"
#include "../attributes/geometry-attributes.h"

namespace svg_cpp_plot {

class Mask : public NodeBase, public Attributes<Mask>, public GeometryAttributes<Mask> {
public:
	Mask() : NodeBase("mask") { GeometryAttributes<Mask>::x(Length(0,percentage)).y(Length(0,percentage)).width(Length(100,percentage)).height(Length(100,percentage)); }
};

template<typename T,typename FCombine>
class MaskGenerator : public NodeGenerator<T,FCombine>, public Attributes<MaskGenerator<T,FCombine>>, public GeometryAttributes<MaskGenerator<T,FCombine>> {
public:
	MaskGenerator(const T& t = T(), const FCombine& combine = FCombine()) : NodeGenerator<T,FCombine>("mask",t,combine) 
	{ GeometryAttributes<MaskGenerator<T,FCombine>>::x(Length(0,percentage)).y(Length(0,percentage)).width(Length(100,percentage)).height(Length(100,percentage)); }		
};


}
