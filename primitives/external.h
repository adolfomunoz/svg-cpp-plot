#pragma once

#include "../core/node.h"

namespace svg_cpp_plot {

inline auto external_svg(const std::string& filename) {
	Node object("object");
	object.set("type","image/svg+xml").set("data",filename).set("width","100%").set("height","100%");
	object.add(Node("img")).set("src",filename);
	return object;
}

};
