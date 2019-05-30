#pragma once

#include "function-image.h"

namespace svg_cpp_plot {
namespace _2d {
	
template<typename F, typename C>
auto function_2d(const F& f, const C& c, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const std::tuple<unsigned int,unsigned int> nsamples = {100,100}) {
	static_assert(std::is_floating_point_v<decltype(f(std::get<0>(xmin),std::get<1>(xmin)))>, "Function f should return floating point");	
	static_assert(is_3d_point_v<decltype(c(float(0)))>,
	       "Function c (color table) should return a color tuple (3d)");	

	return function_image([f,c] (float x, float y) { return c(f(x,y)); },xmin,xmax,nsamples);
}

class color_map_red_blue {
	float tmin, tmax, tmid;
public:
	color_map_red_blue(float tmin = -1.0f, float tmax = 1.0f) : 
	tmin(tmin), tmax(tmax), tmid(tmax+tmin/2.0f) {}
	
	std::tuple<float,float,float> operator()(float t) const noexcept {
		if (t>tmax) return std::tuple(0.0f,0.0f,1.0f);
		else if (t<tmin) return std::tuple(1.0f,0.0f,0.0f);
		else if (t>tmid) {
			float s = (t-tmid)/(tmax-tmid);
			return std::tuple(1.0f-s,1.0f-s,1.0f);
		} else { // tmin < t <= tmid
			float s = (t - tmin)/(tmid-tmin);
			return std::tuple(1.0f,s,s);
		}	
	}
};

class color_map_grayscale {
	float tmin, tmax;
public:
	color_map_grayscale(float tmin = 0.0f, float tmax = 1.0f) :
		tmin(tmin), tmax(tmax) {}
	std::tuple<float,float,float> operator()(float t) const noexcept {
		if (t<tmin) return std::tuple(0.0f,0.0f,0.0f);
		else if (t>tmax) return std::tuple(1.0f,1.0f,1.0f);
		else {
			float v = (t-tmin)/(tmax-tmin);
			return std::tuple(v,v,v);
		}
	}
};

template<typename F>
auto function_2d(const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const std::tuple<unsigned int,unsigned int> nsamples = {100,100}) {
	return function_2d(f,color_map_grayscale(),xmin,xmax,nsamples);
}

}
}
