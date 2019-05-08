#pragma once

#include "../primitives/path.h"
#include <cmath>
#include <type_traits>

namespace svg_cpp_plot {


//OK SO THIS WAS NOT WORKING AS EXPECTED BECAUSE MANY NEGATIVE NUMBERS APPEARED AND BECAUSE OFTEN WE WILL USE THIS IN A GRAPH WHERE THE LOWER PART IS 0 AND THE UPPER PART IS NOT
/*
 * We eliminate this as it is now and just plot things as is. This means that plotting functions without a graph will look weird (y = 0 will be up)
template<typename T>
std::tuple<float, float> point_to_path(const T& t) {
	static_assert(is_2d_point_v<T>, "Expecting a two dimensional point");
	return std::tuple<float,float>(std::get<0>(t), -std::get<1>(t));
}
*/
template<typename T>
std::tuple<float, float> point_to_path(const T& t) {
	static_assert(is_2d_point_v<T>, "Expecting a two dimensional point");
	return t; 
}


//Took a while but I have nailed the maths. We need the derivative in this case but it is calculated numerically below 
//if needed
template<typename F, typename DF>
Path plot_curve_derivative(const F& f, const DF& df, float tmin, float tmax, unsigned int nsamples = 100) {
	static_assert(is_2d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
	static_assert(is_2d_point_v<decltype(df(tmin))>, "Derivative df should return a two dimensional point");
	float dt = (tmax - tmin)/float(nsamples-1);
	float t;
	Path path(point_to_path(f(tmin)));
	for (t=tmin; t<(tmax-0.5*dt); t+=dt) {
		path.curve_to(point_to_path(f(t)+df(t)*(dt/3.0f)),
			      point_to_path(f(t+dt) - df(t+dt)*(dt/3.0f)),
			      point_to_path(f(t+dt)));
	}
	return path.stroke_linecap(stroke_linecap_round);
}

template<typename F>
Path plot_curve(const F& f, float tmin, float tmax, unsigned int nsamples = 100) {
	float dt = (tmax - tmin)/float(nsamples-1);
	return plot_curve_derivative(f,[&f,dt] (float t) { return (f(t+0.05f*dt)-f(t))/(0.05f*dt); },tmin, tmax, nsamples);
}

}
