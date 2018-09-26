#pragma once

#include "transform.h"
#include "plot-curve.h"
#include "../core/group-z-ordered.h"
#include "../core/style.h"
#include "transform.h"

namespace svg_cpp_plot {

	
class Graph3D : public GroupZOrdered {//, public Attributes<Graph3D>, public StyleAttributes<Graph3D>, public PresentationAttributes<Graph3D> {
	Matrix matrix;
	float z_threshold;
	float dz_threshold;
	int nplots, nlines, npolygons;
	Style& style;
	
	template<typename F, typename DF, typename P, typename DP>
	void plot_curve_3d_slice(const F& f, const DF& df, float tmin, float tmax, 
							 const P& pmin, const P& pmax, const DP& dpmin, const DP& dpmax, float dt_min, const std::string& classname) {
		//pmin, pmax, dpmin, dpmax are already transformed
		float dt = tmax - tmin;		
		if ( (dt<=dt_min) ||
		     ( (std::abs(std::get<2>(pmin)-std::get<2>(pmax))<=z_threshold) && 
			   (std::abs(std::get<2>(dpmin)-std::get<2>(dpmax))<=dz_threshold) ) ) {
			float z = (std::get<2>(pmin) + std::get<2>(pmax))/2.0f;
			this->add(Path(point_to_path(point_3d_to_2d(pmin))),z)
				.curve_to(point_to_path(point_3d_to_2d(pmin)+point_3d_to_2d(dpmin)*dt/3.0),
						point_to_path(point_3d_to_2d(pmax)-point_3d_to_2d(dpmax)*dt/3.0),
						point_to_path(point_3d_to_2d(pmax)))
				.class_(classname);
		} else {
			float tmed = (tmin + tmax)/2.0f;
			P pmed = transform_point(matrix,f(tmed)); 
			DP dpmed = transform_direction(matrix,df(tmed));
			plot_curve_3d_slice(f,df,tmin,tmed,pmin,pmed,dpmin,dpmed,dt_min,classname);
			plot_curve_3d_slice(f,df,tmed,tmax,pmed,pmax,dpmed,dpmax,dt_min,classname);
		}
	}

	template<typename F, typename DF>
	void curve_derivative_3d(const F& f, const DF& df, float tmin, float tmax, int min_samples, int max_samples, const std::string& classname) {
		if (min_samples<2) min_samples = 2;
		if (max_samples<2) max_samples = 2;
		float dt_min = (tmax - tmin)/float(max_samples-1);
		float dt = (tmax - tmin)/float(min_samples-1);
		float t;
		for (t=tmin;t<(tmax-0.5*dt); t+=dt)
			plot_curve_3d_slice(f,df,t,t+dt,
				transform_point(matrix,f(t)),transform_point(matrix,f(t+dt)),
				transform_direction(matrix,df(t)),transform_direction(matrix,df(t+dt)),
				dt_min, classname);
	}

	template<typename P0, typename P1>
	void line_simple(const P0& p0, const P1& p1, int max_samples, const std::string& classname) {
		curve_derivative_3d([p0, p1] (float t) { return p0*(1.0f-t) + p1*t; }, [p0, p1] (float t) { return p1-p0; }, 0, 1, 2, max_samples, classname);
	}


public:
	Graph3D(const Matrix& matrix, float z_threshold = 0.1f, float dz_threshold = 0.2f) :
		matrix(matrix), z_threshold(z_threshold), dz_threshold(dz_threshold), nplots(0), nlines(0), npolygons(0), 
		style(GroupZOrdered::add(Style(),1.e10f))	{ }
		
	template<typename F, typename DF>
	StyleEntry& plot_curve_derivative_3d(const F& f, const DF& df, float tmin, float tmax, int min_samples = 10, int max_samples = 10000) {
		static_assert(is_3d_point_v<decltype(f(tmin))>, "Function f should return a three dimensional point");
		static_assert(is_3d_point_v<decltype(df(tmin))>, "Derivative df should return a three dimensional point");
		std::string classname = std::string("plot")+std::to_string(++nplots);
		curve_derivative_3d(f, df, tmin, tmax, min_samples, max_samples, classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}

	template<typename F>
	StyleEntry& plot_curve_3d(const F& f, float tmin, float tmax, int min_samples = 10,int max_samples = 10000) {
		static_assert(is_3d_point_v<decltype(f(tmin))>, "Function f should return a three dimensional point");
		float dt = (tmax - tmin)/float(max_samples-1);
		return this->plot_curve_derivative_3d(f, [&f,dt] (float t) { return (f(t+0.25f*dt) - f(t))/(0.25f*dt); },
			tmin, tmax, min_samples, max_samples);
	}

	template<typename P0, typename P1>
	StyleEntry& line(const P0& p0, const P1& p1, int max_samples = 10000) {
		static_assert(is_3d_point_v<P0> && is_3d_point_v<P1>, "Line should connect two 3D points");
		std::string classname = std::string("line")+std::to_string(++nlines);
		line_simple(p0,p1,max_samples, classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}
	
	//This is so it can be called with ({ }, { })
	StyleEntry& line(const std::tuple<float, float, float>& p0, const std::tuple<float, float, float>& p1, int max_samples = 10000) {
		std::string classname = std::string("line")+std::to_string(++nlines);
		line_simple(p0,p1,max_samples, classname);
		return style.add_class(classname).stroke_linecap(round).fill(none);
	}
};
	
};
