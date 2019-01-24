#pragma once

#include "transform.h"
#include "plot-curve.h"
#include "../core/group.h"
#include "../core/style.h"
#include "transform.h"
#include <cassert>

namespace svg_cpp_plot {

	
class Graph2D : public Group {
	std::tuple<float, float> size; //<-- The group's bounding box should be (0,0) to size
	BoundingBox bb; //<-- This is the bounding box in world's coordinates, that is fit into (0,0) size
	float border_threshold;
	int nplots;
	Style& style;

	template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
	bool is_inside(const P& p) {
		return (std::get<0>(p) >= 0.0f) &&
		       (std::get<1>(p) >= 0.0f) &&
		       (std::get<0>(p) <= std::get<0>(size)) &&
		       (std::get<1>(p) <= std::get<1>(size));
	}


	template<typename F, typename Pin, typename Pout>
	float bolzano_border(const F& f, float tin, float tout, const Pin& pin, const Pout& pout) {
		//These will be disabled when not in debug mode
		assert(is_inside(pin)); assert(!is_inside(pout));
		assert(pin == f(tin));  assert(pout == f(tout));
		//End of assertions
		
		if (distance(pin,pout)<=border_threshold) return tin;
		else {
			float tmed = (tin + tout)/2.0;
			auto pmed = f(tmed);
			if (is_inside(pmed)) return bolzano_border(f,tmed,tout,pmed,pout);
			else return bolzano_border(f,tin,tmed,pin,pmed);
		}
	}

	template<typename F, typename DF>
	void plot_curve_local(const F& f, const DF& df, float tmin, float tmax, unsigned int nsamples, const std::string& classname) {
		float dt = (tmax - tmin)/float(nsamples-1);
		auto p0 = f(tmin);
		Path path(p0); 
		float t;
		for (t = tmin; t<(tmax-0.5*dt); t+=dt) {
			if ((t+dt)>tmax) dt = (tmax - t);
			auto p1 = f(t+dt);
			if (is_inside(p0) && (is_inside(p1)) ) { // We continue with the same path
				path.curve_to(p0+df(t)*(dt/3.0f), p1 - df(t+dt)*(dt/3.0f), p1);
			} else if (is_inside(p0) && (!is_inside(p1))) { //We find the bolzano t, end the path and store it
				float tend = bolzano_border(f, t, t+dt,p0,p1);
				auto pend = f(tend);
				float dtend = tend - t;
				path.curve_to(p0+df(t)*(dtend/3.0f), pend - df(tend)*(dtend/3.0f),pend);
				this->add(path).class_(classname);
			} else if (!is_inside(p0) && (is_inside(p1))) { //We find the bolzano t and start a new path
				float tstart = bolzano_border(f,t+dt,t,p1,p0);
				auto pstart = f(tstart);
				path = Path(pstart);
			} //if both of them are outside we do nothing
			p0 = p1;
		}
		//We have finished a path, we store it.
		if (is_inside(f(t)) && is_inside(f(tmax))) this->add(path).class_(classname);
	}

	template<typename F, typename DF>
	void plot_curve_global(const F& f, const DF& df, float tmin, float tmax, unsigned int nsamples, const std::string& classname) {
		plot_curve_local(
			[&] (float t) { return size*((f(t) - bb.min())/(bb.max() - bb.min())); },
			[&] (float t) { return size*df(t)/(bb.max() - bb.min()); },
		        tmin, tmax,nsamples, classname);
	}

	template<typename P0, typename P1, typename = std::enable_if_t<is_2d_point_v<P0> && is_2d_point_v<P1>> >
	void line(const P0& p0, const P1& p1, const std::string& classname) {
		plot_curve_global([&] (float t) { return p0*(1-t) + p1*t; }, [&] (float t) { return p1-p0; }, 0.0f, 1.0f, 2, classname);
	}

	//So it can be called with {} {}
	void line(const std::tuple<float,float>& p0, const std::tuple<float,float>& p1, const std::string& classname) {
		plot_curve_global([&] (float t) { return p0*(1-t) + p1*t; }, [&] (float t) { return p1-p0; }, 0.0f, 1.0f, 2, classname);
	}

	template<typename Points>
	void polyline(const Points& ps, int max_samples, const std::string& classname) {
		auto p0 = std::begin(ps);
		auto p1 = p0; ++p1;
		for (; p1 != std::end(ps); ++p0, ++p1) line(*p0,*p1, classname);
	}

public:
	Graph2D(const std::tuple<float, float>& size, const BoundingBox& bb, float border_threshold = 1.e-3f):
	       size(size),bb(bb),border_threshold(border_threshold), nplots(0), style(Group::add(Style())) 
	{ }
		
	template<typename F, typename DF>
	StyleEntry& plot_curve_derivative(const F& f, const DF& df, float tmin, float tmax, int nsamples = 100) {
		static_assert(is_2d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
		static_assert(is_2d_point_v<decltype(df(tmin))>, "Derivative df should return a two dimensional point");
		std::string classname = std::string("plot")+std::to_string(++nplots);
		plot_curve_global(f, df, tmin, tmax, nsamples, classname);
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
	StyleEntry& plot_function_derivative(const F& f, const DF& df, int nsamples = 100) {
		static_assert(std::is_floating_point_v<decltype(f(float(0)))>, "Function f should return floating point");
		static_assert(std::is_floating_point_v<decltype(df(float(0)))>, "Function df should return floating point");
		return plot_curve_derivative([&f] (float t) { return std::tuple<float,float>(t,f(t)); },
			[&df] (float t) { return std::tuple<float,float>(1,df(t)); }, std::get<0>(bb.min()), std::get<0>(bb.max()), nsamples);
	}

	template<typename F>
	StyleEntry& plot_function(const F& f, int nsamples = 100) {
		static_assert(std::is_floating_point_v<decltype(f(float(0)))>, "Function f should return floating point");
		float dx = (std::get<0>(bb.max()) - std::get<0>(bb.min()))/float(nsamples-1);
		return plot_function_derivative(f, [&f,dx] (float x) { return (f(x+0.25f*dx) - f(x))/(0.25f*dx); }, nsamples);
	}

	StyleEntry& axis() {
		std::string classname = "axis";
		line({0.0f,std::get<1>(bb.min())},{0.0f,std::get<1>(bb.max())},classname);
		line({std::get<0>(bb.min()),0.0f},{std::get<0>(bb.max()),0.0f},classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}

	StyleEntry& border() {
		std::string classname = "axis";
		line({0.0f,std::get<1>(bb.min())},{0.0f,std::get<1>(bb.max())},classname);
		line({std::get<0>(bb.min()),0.0f},{std::get<0>(bb.max()),0.0f},classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}



};
	
};
