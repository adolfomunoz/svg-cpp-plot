#pragma once

#include <list>
#include <tuple>
#include <string>
#include "../../2d/function-2d.h"

namespace svg_cpp_plot {

class ImShow : public Generator<_2d::Matrix> {
	std::vector<std::vector<float>> data;
	std::string cmap_;
public:
	ImShow(const std::vector<std::vector<float>>& data) : data(data) {}
	
	ImShow& cmap(std::string_view s) { cmap_=s; return *this; }
	std::string_view cmap() const { return cmap_; }
	
	std::tuple<std::size_t,std::size_t> size() const {
		std::size_t y(0);
		for (const auto& r : data) if (r.size()>y) y=r.size();
		return std::tuple<std::size_t,std::size_t>(data.size(),y);
	}
	
	std::array<float,4> axis() const {
		return std::array<float,4>{
				-0.5f,float(std::get<0>(size()))-0.5f,
				-0.5f,float(std::get<1>(size()))-0.5f};
	}
	
	std::string to_string(const _2d::Matrix& m) const noexcept override {
		_2d::color_map cm = _2d::color_map_viridis();
		if (cmap() == "magma") cm = _2d::color_map_magma();
		else if (cmap() == "grayscale") cm = _2d::color_map_grayscale();
		float max_value = 0.0f;
		for (const auto& row : data) for (const auto& v : row)
			if (v > max_value) max_value = v;
		return _2d::function_2d([&] (float x, float y) {
			if ((x<0) || (y<0) || (data.size()<y) || (data[std::floor(y+0.5f)].size() < x)) return 0.0f;
			else return data[std::floor(y+0.5f)][std::floor(x+0.5f)]/max_value; },
			cm, {axis()[0],axis()[2]}, {axis()[1],axis()[3]}, _2d::image_strategy::sharp_pixels(size(),0)).to_string(m);
	}
};
	
}