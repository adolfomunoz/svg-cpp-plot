#pragma once

#include "curve.h"
#include <cmath>

namespace svg_cpp_plot {
namespace _2d {
	
template<typename F, typename DF>
auto function_derivative(const F& f, const DF& df, float xmin, float xmax, unsigned int nsamples = 100) {
	static_assert(std::is_floating_point_v<decltype(f(xmin))>,
	       "Function f should be a unary floating point function returning floating point");	
	static_assert(std::is_floating_point_v<decltype(f(xmin))>,
	       "Derivative df should be a unary floating point function returning floating point");

	return curve_derivative([f] (float x) { return std::tuple(x,f(x)); }, [df] (float x) { return std::tuple(1.0f,df(x)); },xmin, xmax, nsamples);
}


template<typename F>
auto function(const F& f, float xmin, float xmax, unsigned int nsamples = 100) {
	float dx = (xmax - xmin)/float(nsamples-1);
	return function_derivative(f,[f,dx] (float x) { return (f(x+0.05f*dx)-f(x))/(0.05f*dx); },xmin, xmax, nsamples);
}


}
}
