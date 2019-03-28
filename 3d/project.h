#pragma once

#include "point-3d.h"

namespace svg_cpp_plot {
template<typename F> 
auto project_to_x(const F& f, float x) {
	return [f,x] (float t) {
		auto p = f(t);
		return std::tuple(x, std::get<1>(p), std::get<2>(p));
	};
}

template<typename F> 
auto project_to_y(const F& f, float y) {
	return [f,y] (float t) {
		auto p = f(t);
		return std::tuple(std::get<0>(p), y, std::get<2>(p));
	};
}

template<typename F> 
auto project_to_z(const F& f, float z) {
	return [f,z] (float t) {
		auto p = f(t);
		return std::tuple(std::get<0>(p), std::get<1>(p), z);
	};
}
};
