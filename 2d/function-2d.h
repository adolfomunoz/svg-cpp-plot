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
		float pos = steps.size()*((t-tmin)/(tmax-tmin));
		int s = std::floor(pos);
		if (s < 0) return steps.front();
		else if (std::size_t(s) >= steps.size()) return steps.back();
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

template<typename F>
auto function_2d(const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const std::tuple<unsigned int,unsigned int> nsamples = {100,100}) {
	return function_2d(f,color_map_grayscale(),xmin,xmax,nsamples);
}

}
}
