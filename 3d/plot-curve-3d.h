#pragma once

#include "../primitives/path.h"
#include <cmath>
#include <type_traits>
#include "../plot/plot-curve.h"
#include "transform.h"

namespace svg_cpp_plot {


template<typename F, typename DF>
Path plot_curve_derivative_3d(const F& f, const DF& df, float tmin, float tmax, const Matrix& m, unsigned int nsamples = 100) {
	static_assert(is_3d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
	static_assert(is_3d_point_v<decltype(df(tmin))>, "Derivative df should return a two dimensional point");
	return plot_curve_derivative([&f,&m] (float t) { return point_3d_to_2d(transform_point(m,f(t))); }, 
			  [&df, &m] (float t) { return point_3d_to_2d(transform_direction(m,df(t))); }, tmin, tmax, nsamples);
}

template<typename F>
Path plot_curve_3d(const F& f, float tmin, float tmax, const Matrix& m, unsigned int nsamples = 100) {
	static_assert(is_3d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
	return plot_curve_derivative([&f,&m] (float t) { return point_3d_to_2d(transform_point(m,f(t))); }, tmin, tmax, nsamples);
}

}
