#pragma once
#include "../primitives/svg.h"
#include "../scripts/for-all.h"
#include "../scripts/set-interval.h"
#include "../scripts/reload.h"
#include <fstream>


namespace svg_cpp_plot {
void watch_image(const char* url) {
	std::string wrapper_name = std::string(url)+".wrap.svg";
	SVG svg;
	Image& img = svg.add(Image(url)).rect(0,0,1280,720);
	svg.add(Script()).add(script::SetInterval(script::ForAll(std::string("#")+img.id(),script::Reload()),0.5));
	std::ofstream fw(wrapper_name);
	fw<<svg.automatic_viewBox();
	fw.close();
	browse(wrapper_name.c_str());
}
}

