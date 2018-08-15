#pragma once

#include "../core/path.h"
#include <cmath>

namespace svg_cpp_plot {

//Took a while but I have nailed the maths. We need the derivative in this case but it is calculated numerically below 
//if needed
template<typename F, typename DF>
Path plot_function(const F& f, const DF& df, float xmin, float xmax, unsigned int nsamples = 100) {
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

}
