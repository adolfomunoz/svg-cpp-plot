#pragma once

#include "../primitives/path.h"
#include <cmath>
#include "plot-curve.h"

namespace svg_cpp_plot {

template<typename F, typename DF>
Path plot_function_derivative(const F& f, const DF& df, float xmin, float xmax, unsigned int nsamples = 100) {
	static_assert(std::is_floating_point_v<decltype(f(xmin))>,
	       "Function f should be a unary floating point function returning floating point");	
	static_assert(std::is_floating_point_v<decltype(f(xmin))>,
	       "Derivative df should be a unary floating point function returning floating point");
	
	return plot_curve_derivative([&f] (float x) { return std::tuple(x,f(x)); }, [&df] (float x) { return std::tuple(1.0f,df(x)); },
		xmin, xmax, nsamples);
}	

template<typename F>
Path plot_function(const F& f, float xmin, float xmax, unsigned int nsamples = 100) {
	float dx = (xmax - xmin)/float(nsamples-1);
	return plot_function_derivative(f,[&f,dx] (float x) { return (f(x+0.05f*dx)-f(x))/(0.05f*dx); },xmin, xmax, nsamples);
}

/*
//Took a while but I have nailed the maths. We need the derivative in this case but it is calculated numerically below 
//if needed
template<typename F, typename DF>
Path plot_function(const F& f, const DF& df, float xmin, float xmax, unsigned int nsamples = 100) {
	static_assert(std::is_floating_point_v<decltype(f(xmin))>,
	       "Function f should be a unary floating point function returning floating point");	
	static_assert(std::is_floating_point_v<decltype(f(xmin))>,
	       "Derivative df should be a unary floating point function returning floating point");	
	float dx = (xmax - xmin)/float(nsamples-1);
	float x;
	Path path(xmin, -f(xmin));
	for (x=xmin; x<(xmax-0.5*dx); x+=dx) {
		path.curve_to(x+dx/3.0f,   -(f(x) + dx*df(x)/3.0f),
			      x+2.0f*dx/3.0f,-(f(x+dx) - dx*df(x+dx)/3.0),
			      x+dx,-f(x+dx));
	}
	return path;
}

template<typename F>
Path plot_function(const F& f, float xmin, float xmax, unsigned int nsamples = 100) {
	float dx = (xmax - xmin)/float(nsamples-1);
	return plot_function(f,[&f,dx] (float x) { return (f(x+0.05f*dx)-f(x))/(0.05f*dx); },xmin, xmax, nsamples);
}
*/

}
