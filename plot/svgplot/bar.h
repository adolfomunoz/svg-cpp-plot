#pragma once

#include <list>
#include <tuple>
#include <string>
#include "../../2d/transform.h"
#include "../../2d/points.h"
#include "../../2d/polyline.h"

namespace svg_cpp_plot {

class Bar : public Generator<_2d::Matrix>  {
    
    std::vector<float> x;
    std::vector<float> height;
    std::vector<float> width;
    std::vector<float> bottom;
	const Color* color_;
public:
	Bar(const std::vector<float>& x, const std::vector<float>& height) : x(x), height(height), width(1,0.8f), bottom(1,0.0f), color_(nullptr) {}

    Bar& width(const std::vector<float>& w) {
        width = w; return *this;
    }
    
    Bar& width(float w) {
        return width(std::vector<float>{w});
    }
    

	Plot& color(const Color& c) { color_=&c; return *this; }
	const Color& color() const { return color_?*color_:black; }
	
	std::string to_string(const _2d::Matrix& m) const noexcept override {

	}
};

class PlotGroup : public std::list<Plot> {
public:
	using std::list<Plot>::list;
	PlotGroup& format(std::string_view f) { 
		for (Plot& p : (*this)) p.format(f); 
		return *this;
	}
	PlotGroup& linewidth(float f) { 
		for (Plot& p : (*this)) p.linewidth(f); 
		return *this;
	}
	PlotGroup& color(const Color& c) {
		for (Plot& p : (*this)) p.color(c);  
		return *this; 
	}
		
	Plot& add(const Plot& plot) {
		this->push_back(plot); 
		return this->back();
	}
};
	
}