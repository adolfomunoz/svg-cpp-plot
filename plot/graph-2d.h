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
	int nplots, npoints;
	Style& style;

	template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
	bool is_inside(const P& p) {
		return (std::get<0>(p) >= 0.0f) &&
		       (std::get<1>(p) >= 0.0f) &&
		       (std::get<0>(p) <= std::get<0>(size)) &&
		       (std::get<1>(p) <= std::get<1>(size));
	}

	template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
	void point_local(const P& p, float radius, const std::string& classname) {
		this->add(Circle(p,radius)).class_(classname);	
	}

	template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
	void point_global(const P& p, float radius, const std::string& classname) {
		std::tuple<float, float> plocal(
			std::get<0>(size)*( (std::get<0>(p) - std::get<0>(bb.min()))/(std::get<0>(bb.max())-std::get<0>(bb.min())) ),
			std::get<1>(size)*(1.0f - (std::get<1>(p) - std::get<1>(bb.min()))/(std::get<1>(bb.max())-std::get<1>(bb.min())) ));
		if (is_inside(plocal)) point_local(plocal, radius, classname);	
	}

	template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
	void text_local(const P& p, std::string text, const std::string& classname) {
		this->add(Text(std::get<0>(p), std::get<1>(p), text)).class_(classname);
	}

	void text_local(const std::tuple<float,float>& p, std::string text, const std::string& classname) {
		this->add(Text(std::get<0>(p), std::get<1>(p), text)).class_(classname);
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
			[&] (float t) {
		       		auto p = (f(t) - bb.min())/(bb.max() - bb.min()); std::get<1>(p) = 1.0f - std::get<1>(p);	
				return size*p; },
			[&] (float t) { 
				auto d = df(t)/(bb.max() - bb.min()); std::get<1>(d)*=-1.0f;
				return size*d; },
		        tmin, tmax,nsamples, classname);
	}

	template<typename P0, typename P1, typename = std::enable_if_t<is_2d_point_v<P0> && is_2d_point_v<P1>> >
	void line_local(const P0& p0, const P1& p1, const std::string& classname) {
		this->add(Line(std::get<0>(p0),std::get<1>(p0), std::get<0>(p1),std::get<1>(p1))).class_(classname);
	}

	//So it can be called with {} {}
	void line_local(const std::tuple<float,float>& p0, const std::tuple<float,float>& p1, const std::string& classname) {
		this->add(Line(std::get<0>(p0),std::get<1>(p0), std::get<0>(p1),std::get<1>(p1))).class_(classname);
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
	void polyline(const Points& ps, const std::string& classname) {
		auto p0 = std::begin(ps);
		auto p1 = p0; ++p1;
		for (; p1 != std::end(ps); ++p0, ++p1) line(*p0,*p1, classname);
	}

	void polyline(std::initializer_list<std::tuple<float,float>> ps, const std::string& classname) {
		auto p0 = std::begin(ps);
		auto p1 = p0; ++p1;
		for (; p1 != std::end(ps); ++p0, ++p1) line(*p0,*p1, classname);
	}

	template<typename Points>
	void polygon(const Points& ps, const std::string& classname) {
		auto p0 = std::begin(ps);
		auto p1 = p0; ++p1;
		for (; p1 != std::end(ps); ++p0, ++p1) line(*p0,*p1, classname);
		line(*p0,*(std::begin(ps)),classname);
	}

	void polygon(std::initializer_list<std::tuple<float,float>> ps, const std::string& classname) {
		auto p0 = std::begin(ps);
		auto p1 = p0; ++p1;
		for (; p1 != std::end(ps); ++p0, ++p1) line(*p0,*p1, classname);
		line(*p0,*(std::begin(ps)),classname);
	}


	void xtick(float xlocal, float height, float ylocal, const std::string& classname) {
		line_local({xlocal,ylocal-0.5f*height},{xlocal,ylocal+0.5f*height},classname);
	}

	void ytick(float ylocal, float width, float xlocal, const std::string& classname) {
		line_local({xlocal-0.5f*width,ylocal},{xlocal+0.5f*width,ylocal},classname);
	}



public:
	Graph2D(const std::tuple<float, float>& size, const BoundingBox& bb, float border_threshold = 1.e-3f):
	       size(size),bb(bb),border_threshold(border_threshold), nplots(0), npoints(0),style(Group::add(Style())) 
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

	template<typename Points>
	StyleEntry& plot_points(const Points& ps, float radius = 0) {
		static_assert(is_2d_point_v<std::decay_t<decltype(*(std::begin(ps)))>>, "List of points should contain 2d points");
		std::string classname = std::string("points")+std::to_string(++npoints);
		for (auto p : ps) point_global(p,radius,classname);
		return style.add_class(classname);
	}

	StyleEntry& plot_points(std::initializer_list<std::tuple<float,float>> ps, float radius = 0) {
		return plot_points(std::list<std::tuple<float,float>>(ps), radius);
	}

	StyleEntry& axis(float x = 0.0f, float y = 0.0f, std::string classname = "axis") {
		line({x,std::get<1>(bb.min())},{x,std::get<1>(bb.max())},classname);
		line({std::get<0>(bb.min()),x},{std::get<0>(bb.max()),x},classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}

	StyleEntry& border(std::string classname = "border") {
		polygon({{std::get<0>(bb.min()),std::get<1>(bb.min())},
			  {std::get<0>(bb.min()),std::get<1>(bb.max())},
			  {std::get<0>(bb.max()),std::get<1>(bb.max())},
			  {std::get<0>(bb.max()),std::get<1>(bb.min())}},classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}

	StyleEntry& xticks(int count = 2, float height = 3.0f, float ylocal = 0.0f, std::string classname = "xticks") {
		float dx = std::get<0>(size)/float(count - 1);
		for (int i = 0;i<count;++i) xtick(i*dx, height, std::get<1>(size) - ylocal, classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}

	StyleEntry& yticks(int count = 2, float width = 3.0f, float xlocal = 0.0f, std::string classname = "yticks") {
		float dy = std::get<1>(size)/float(count - 1);
		for (int i = 0;i<count;++i) ytick(i*dy, width, xlocal, classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}

	StyleEntry& ticks(int xs = 2, int ys = 2, float size = 3.0f, float xlocal = 0.0f, float ylocal = 0.0f, std::string classname = "ticks") {
		xticks(xs,size,ylocal, classname);
		return yticks(ys,size,xlocal,classname);
	}

	StyleEntry& xlabels(int count = 2, float ylocal = -3.0f, std::string classname = "xlabels") {
		float dxlocal = std::get<0>(size)/float(count - 1);
		float dx = (std::get<0>(bb.max())-std::get<0>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<0>(bb.min())+i*dx);
			text_local({i*dxlocal,std::get<1>(size) - ylocal},stext.str(),classname);
		}
		return style.add_class(classname).text_anchor(middle).font_size(float(std::get<1>(size))/float(20)).alignment_baseline(hanging);
	}

	StyleEntry& ylabels(int count = 2, float xlocal = -3.0f, std::string classname = "ylabels") {
		float dylocal = std::get<1>(size)/float(count - 1);
		float dy = (std::get<1>(bb.max())-std::get<1>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<1>(bb.min())+i*dy);
			text_local({xlocal, std::get<1>(size) - i*dylocal},stext.str(),classname);
		}
		return style.add_class(classname).text_anchor(end).font_size(float(std::get<1>(size))/float(20)).alignment_baseline(baseline_middle);
	}

};
	
};
