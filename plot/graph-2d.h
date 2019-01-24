#pragma once

#include "transform.h"
#include "plot-curve.h"
#include "../core/group.h"
#include "../core/style.h"
#include "transform.h"

namespace svg_cpp_plot {

	
class Graph2D : public Group {
	std::tuple<float, float> size; //<-- The group's bounding box should be (0,0) to size
	BoundingBox bb; //<-- This is the bounding box in world's coordinates, that is fit into (0,0) size
	int nplots;
	Style& style;

public:	
	template<typename F, typename DF>
	void plot_curve_local(const F& f, const DF& df, float tmin, float tmax, unsigned int nsamples, const std::string& classname) {	
		 return svg_cpp_plot::plot_curve_derivative(f,df,tmin,tmax,nsamples).class_(classname);
	}

	template<typename F, typename DF>
	void plot_curve_global(const F& f, const DF& df, float tmin, float tmax, unsigned int nsamples, const std::string& classname) {
		return plot_curve_local(
			[&f,&size,&bb] (float t) { return size*((f(t) - bb.min())/(bb.max() - bb.min())); },
			[&df,&size,&bb] (float t) { return size*df(t)/(bb.max() - bb.min()); },
		        tmin, tmax,nsamples, classname);
	}

public:
	Graph2D(const std::tuple<float, float>& size, const BoundingBox& bb, float border_threshold = 1.e-5f):
	       size(size),bb(bb),border_threshold(border_threshold), nplots(0), style(Group::add(Style())) { }
		
	template<typename F, typename DF>
	StyleEntry& plot_curve_derivative(const F& f, const DF& df, float tmin, float tmax, int nsamples = 100) {
		static_assert(is_2d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
		static_assert(is_2d_point_v<decltype(df(tmin))>, "Derivative df should return a two dimensional point");
		std::string classname = std::string("plot")+std::to_string(++nplots);
		plot_curve_global(f, df, tmin, tmax, min_samples, max_samples, classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}

	template<typename F>
	StyleEntry& plot_curve(const F& f, float tmin, float tmax, int nsamples = 100) {
		static_assert(is_2d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
		float dt = (tmax - tmin)/float(nsamples-1);
		return this->plot_curve_derivative(f, [&f,dt] (float t) { return (f(t+0.25f*dt) - f(t))/(0.25f*dt); },
			tmin, tmax, nsamples);
	}

	template<typename F, typename DF>
	StyleEntry& plot_function_derivative(const F& f, const DF& df, float xmin, float xmax, int nsamples = 100) {
		static_assert(std::is_floating_point_v<decltype(f(xmin))>, "Function f should return floating point");
		static_assert(std::is_floating_point_v<decltype(df(xmin))>, "Function df should return floating point");
		plot_curve_derivative([&f] (float t) { return std::tuple<float,float>(t,f(t)); },
			[&df] (float t) { return std::tuple<float,float>(1,df(t)); }, xmin, xmax, nsamples);

	}

};
	
};
