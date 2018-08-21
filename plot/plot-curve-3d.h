#pragma once

#include "../core/path.h"
#include <cmath>
#include <type_traits>
#include "plot-curve.h"

namespace svg_cpp_plot {

template<typename T>
struct is_3d_point {
	static constexpr bool value = std::tuple_size_v<T> == 3 &&
			std::is_floating_point_v<std::tuple_element_t<0,T>> &&
			std::is_floating_point_v<std::tuple_element_t<1,T>> &&
			std::is_floating_point_v<std::tuple_element_t<2,T>>;
};

template<typename T>
constexpr bool is_3d_point_v = is_3d_point<T>::value;

using Matrix = std::array<std::array<float,4>,4>;

namespace projection {
constexpr Matrix oblique   = { {{1,0,0,0},{0,1,0,0},{1.0/sqrt(2), 1.0/sqrt(2), 0, 0},{0,0,0,1}} };
constexpr Matrix isometric = { {{1.0/sqrt(2),0,-1.0/sqrt(2),0},
	                       {1.0/sqrt(6),2.0/sqrt(6),1.0/sqrt(6),0},
			       {1.0/sqrt(3), -1.0/sqrt(3), 1.0/sqrt(3), 0},
			       {0,0,0,1}} };
}

template<typename P>
std::tuple<float, float> transform_point(const Matrix& m, const P& p) {
	static_assert(is_3d_point_v<P>,"Can only transform 3d points");
	float w = std::get<0>(p)*m[0][3] + std::get<1>(p)*m[1][3] + std::get<2>(p)*m[2][3] + m[3][3];
	return std::tuple<float, float>(
		(std::get<0>(p)*m[0][0] + std::get<1>(p)*m[1][0] + std::get<2>(p)*m[2][0] + m[3][0])/w,
		(std::get<0>(p)*m[0][1] + std::get<1>(p)*m[1][1] + std::get<2>(p)*m[2][1] + m[3][1])/w
		);
}  

template<typename P>
std::tuple<float, float> transform_direction(const Matrix& m, const P& p) {
	static_assert(is_3d_point_v<P>,"Can only transform 3d directions");
	return std::tuple<float, float>(
		(std::get<0>(p)*m[0][0] + std::get<1>(p)*m[1][0] + std::get<2>(p)*m[2][0] + m[3][0]),
		(std::get<0>(p)*m[0][1] + std::get<1>(p)*m[1][1] + std::get<2>(p)*m[2][1] + m[3][1])
		);
}    

template<typename F, typename DF>
Path plot_curve_3d(const F& f, const DF& df, float tmin, float tmax, const Matrix& m, unsigned int nsamples = 100) {
	static_assert(is_3d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
	static_assert(is_3d_point_v<decltype(df(tmin))>, "Derivative df should return a two dimensional point");
	return plot_curve([&f,&m] (float t) { return transform_point(m,f(t)); }, 
			  [&df, &m] (float t) { return transform_direction(m,df(t)); }, tmin, tmax, nsamples);
}

template<typename F>
Path plot_curve_3d(const F& f, float tmin, float tmax, const Matrix& m, unsigned int nsamples = 100) {
	static_assert(is_3d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
	return plot_curve([&f,&m] (float t) { return transform_point(m,f(t)); }, tmin, tmax, nsamples);
}

}
