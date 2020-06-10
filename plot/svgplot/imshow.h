#pragma once

#include <list>
#include <tuple>
#include <string>
#include "../../2d/function-2d.h"

namespace svg_cpp_plot {

class ImShow : public Generator<_2d::Matrix> {
	std::vector<std::vector<float>> data;
	std::string cmap_;
	float vmin_, vmax_; bool vmin_set, vmax_set;
    std::string interpolation_;
public:
	ImShow(const std::vector<std::vector<float>>& data) : data(data), vmin_set(false), vmax_set(false) {}
	
	ImShow& vmin(float f) { vmin_=f; vmin_set=true; return (*this);}
	ImShow& vmax(float f) { vmax_=f; vmax_set=true; return (*this);}

	float vmin() const {
		if (vmin_set) return vmin_;
		else {
			float temp_vmin = data[0][0];
			for (const auto& row : data) for (const auto& v : row)
				if (v < temp_vmin) temp_vmin = v;
			return temp_vmin;
		}
	}
	
	float vmax() const {
		if (vmax_set) return vmax_;
		else {
			float temp_vmax = data[0][0];
			for (const auto& row : data) for (const auto& v : row)
				if (v > temp_vmax) temp_vmax = v;
			return temp_vmax;
		}
	}
	
	ImShow& cmap(std::string_view s) { cmap_=s; return *this; }
	std::string_view cmap() const { return cmap_; }
	ImShow& interpolation(std::string_view s) { interpolation_=s; return *this; }
	std::string_view interpolation() const { return interpolation_; }
	
	std::tuple<std::size_t,std::size_t> size() const {
		std::size_t x(0);
		for (const auto& r : data) if (r.size()>x) x=r.size();
		return std::tuple<std::size_t,std::size_t>(x,data.size());
	}
    
	std::array<float,4> axis() const {
		return std::array<float,4>{
				-0.5f,float(std::get<0>(size()))-0.5f,
				-0.5f,float(std::get<1>(size()))-0.5f};
	}
	
	std::string to_string(const _2d::Matrix& m) const noexcept override {
		_2d::color_map cm = _2d::color_map_viridis(vmin(),vmax());
		if (cmap() == "magma") 
			cm = _2d::color_map_magma(vmin(),vmax());
		else if (cmap() == "grayscale") 
			cm = _2d::color_map_grayscale(vmin(),vmax());

        auto f = [&] (float x, float y) {
			if ((x<0) || (y<0) || (data.size()<y) || (data[std::floor(y+0.5f)].size() < x)) return 0.0f;
			else return data[std::floor(y+0.49f)][std::floor(x+0.5f)]; };
        if ((interpolation()=="") || (interpolation()=="nearest") ||(interpolation()=="none")) {
            return _2d::function_2d(f,cm, {axis()[0],axis()[2]}, {axis()[1],axis()[3]}, _2d::image_strategy::sharp_pixels(size(),0)).to_string(m);
        } else {
            return _2d::function_2d(f,cm, {axis()[0],axis()[2]}, {axis()[1],axis()[3]}, _2d::image_strategy::embedded_png(size())).to_string(m);
        }
	}
};
	
}