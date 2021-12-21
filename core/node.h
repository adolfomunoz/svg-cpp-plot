#pragma once

#include "element.h"

namespace svg_cpp_plot {

class NodeBase : public NotTerminal, public ObjectList<Element> {
public:
	void on_add(Element& e) override { e.parent_ = this; }

	NodeBase(const std::string& tag) : NotTerminal(tag) {}
	
	std::string content() const noexcept override {
		return list_to_string("\n");
	}

	BoundingBox bounding_box() const noexcept override { 
		BoundingBox bb;
		for (auto o : object_list) if (o) bb.join(o->bounding_box());
		bb.expand(get_float("stroke-width",0.0f));
		return bb;
	}
};

class Node : public NodeBase, public Attributes<Node> {
public:
	using NodeBase::NodeBase;
};

}
