#pragma once

#include "polygon.h"
#include <vector>

namespace svg_cpp_plot {
namespace _2d {

template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T> > >
polygon bar_plot(const std::vector<T>& data) {
	float dx = 1.0f/float(data.size());
	polygon b({{0.0f,0.0f}});
	for (std::size_t i = 0;i<data.size();++i) {
		b.add_points({{float(i)*dx,float(data[i])},{float(i+1)*dx,float(data[i])}});
	}
	b.add_points({{1.0f,0.0f}});
	return b;
}	
	
}
}
