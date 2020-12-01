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

color_map color_map_bwr(float tmin = -1.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax, {{0.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f}});
}

color_map color_map_coolwarm(float tmin = -1.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax, {{0.230f, 0.299f, 0.754f},{0.865f, 0.865f, 0.865f},{0.706f, 0.016f, 0.150f}});
}

color_map color_map_seismic(float tmin = -1.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax, {{0.0f,0.0f,0.1f},{0.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f},{0.1f,0.0f,0.0f}});
}

color_map color_map_spectral(float tmin = -1.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax, {{1.0f,0.25f,0.25f},{1.0f,0.75f,0.25f},{1.0f,1.0f,0.75f},{0.25f,0.75f,0.5f},{0.5f,0.25f,0.75f}});
}

color_map color_map_piyg(float tmin = -1.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax, {{0.8f,0.3f,0.6f},{0.9f,0.5f,0.9f},{1.0f,1.0f,1.0f},{0.5f,0.75f,0.0f},{0.0f,0.5f,0.0f}});
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

color_map color_map_inferno(float tmin = 0.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax,
                {   {0.003921568627451,0,0.015686274509804},
                    {0.188235294117647,0.027450980392157,0.329411764705882},
                    {0.411764705882353,0.058823529411765,0.435294117647059},
                    {0.619607843137255,0.156862745098039,0.392156862745098},
                    {0.815686274509804,0.282352941176471,0.262745098039216},
                    {0.937254901960784,0.490196078431373,0.082352941176471},
                    {0.949019607843137,0.76078431372549,0.137254901960784},
                    {0.96078431372549,1,0.63921568627451} });

}

color_map color_map_plasma(float tmin = 0.0f, float tmax = 1.0f) {
	return color_map(tmin,tmax,
                {   {0.184313725490196,0,0.529411764705882},
                    {0.384313725490196,0,0.643137254901961},
                    {0.572549019607843,0,0.650980392156863},
                    {0.729411764705882,0.184313725490196,0.541176470588235},
                    {0.847058823529412,0.356862745098039,0.411764705882353},
                    {0.933333333333333,0.537254901960784,0.286274509803922},
                    {0.964705882352941,0.741176470588235,0.152941176470588},
                    {0.894117647058824,0.980392156862745,0.082352941176471} });
}

template<typename F>
auto function_2d(const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const std::tuple<unsigned int,unsigned int> nsamples = {100,100}) {
	return function_2d(f,color_map_grayscale(),xmin,xmax,nsamples);
}

}
}
