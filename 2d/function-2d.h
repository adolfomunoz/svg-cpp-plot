#pragma once

#include "function-image.h"

namespace svg_cpp_plot {
namespace _2d {
	
template<typename F, typename C, typename Strategy>
auto function_2d(const F& f, const C& c, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const Strategy& strategy) {
	static_assert(std::is_floating_point_v<decltype(f(std::get<0>(xmin),std::get<1>(xmin)))>, "Function f should return floating point");	
	static_assert(is_3d_point_v<decltype(c(float(0)))>,
	       "Function c (color table) should return a color tuple (3d)");	

	return function_image([f,c] (float x, float y) { return c(f(x,y)); },xmin,xmax,strategy);
}

template<typename F, typename C>
auto function_2d(const F& f, const C& c, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const std::tuple<unsigned int,unsigned int> nsamples = {100,100}) {
	static_assert(std::is_floating_point_v<decltype(f(std::get<0>(xmin),std::get<1>(xmin)))>, "Function f should return floating point");	
	static_assert(is_3d_point_v<decltype(c(float(0)))>,
	       "Function c (color table) should return a color tuple (3d)");	

	return function_image([f,c] (float x, float y) { return c(f(x,y)); },xmin,xmax,nsamples);
}


class color_map {
	float tmin; float tmax;
	std::vector<std::tuple<float,float,float>> steps;
public:
	color_map(float tmin, float tmax, const std::initializer_list<std::tuple<float,float,float>>& s) :
		tmin(tmin), tmax(tmax), steps(s) {
			if (steps.size()<1) steps.push_back(std::tuple(0.0f,0.0f,0.0f));
			if (steps.size()<2) steps.push_back(std::tuple(1.0f,1.0f,1.0f));
		}
		
	std::tuple<float,float,float> operator()(float t) const noexcept {
		float pos = (steps.size()-1)*((t-tmin)/(tmax-tmin));
		int s = std::floor(pos);
		if (s < 0) return steps.front();
		else if (std::size_t(s) >= (steps.size()-1)) return steps.back();
		else {
			float t = pos - s; 
			return std::tuple<float,float,float>(std::get<0>(steps[s])*(1.0f-t) + std::get<0>(steps[s+1])*t,
							  std::get<1>(steps[s])*(1.0f-t) + std::get<1>(steps[s+1])*t,
							  std::get<2>(steps[s])*(1.0f-t) + std::get<2>(steps[s+1])*t);
		}	
	}
};

color_map color_map_red_blue(float tmin = -1.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax, {{1.0f,0.0f,0.0f},{1.0f,1.0f,1.0f},{0.0f,0.0f,1.0f}});
}

color_map color_map_grayscale(float tmin = 0.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax, {{0.0f,0.0f,0.0f},{1.0f,1.0f,1.0f}});
}

color_map color_map_heat(float tmin = 0.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax,
				{   {0.0f,0.0f,0.2f},
					{0.0f,0.0f,1.0f},
					{0.0f,1.0f,1.0f},
					{1.0f,1.0f,0.0f},
					{1.0f,0.0f,0.0f},
					{0.4f,0.0f,0.0f} });
}

color_map color_map_viridis(float tmin = 0.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax,
				{   {68.0f/255.0f,1.0f/255.0f,84.0f/255.0f},
					{65.0f/255.0f,68.0f/255.0f,135.0f/255.0f},
					{42.0f/255.0f,120.0f/255.0f,142.0f/255.0f},
					{34.0f/255.0f,168.0f/255.0f,132.0f/255.0f},
					{122.0f/255.0f,209.0f/255.0f,81.0f/255.0f},
					{253.0f/255.0f,231.0f/255.0f,37.0f/255.0f} });
}
color_map color_map_magma(float tmin = 0.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax,
				{   {0.000f,0.000f,0.016f},
					{0.173f,0.067f,0.373f},
					{0.447f,0.122f,0.506f},
					{0.718f,0.216f,0.475f},
					{0.945f,0.376f,0.365f},
					{0.996f,0.682f,0.467f},
					{0.988f,0.992f,0.749f} });
}

template<typename F>
auto function_2d(const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const std::tuple<unsigned int,unsigned int> nsamples = {100,100}) {
	return function_2d(f,color_map_grayscale(),xmin,xmax,nsamples);
}

}
}
