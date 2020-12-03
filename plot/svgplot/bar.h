#pragma once

#include <list>
#include <tuple>
#include <string>
#include "plottable.h"
#include "../../2d/transform.h"
#include "../../2d/points.h"
#include "../../2d/polyline.h"

namespace svg_cpp_plot {

class Bar : public Plottable  {
    std::vector<float> x_;
    std::vector<float> height_;
    std::vector<float> width_;
    std::vector<float> bottom_;
	const Color* color_;
public:
	Bar(const std::vector<float>& x, const std::vector<float>& height) : x_(x), height_(height), width_(1,0.8f), bottom_(1,0.0f), color_(nullptr) {}

    const std::vector<float>& x() const {
        return x_;
    }

    float x(std::size_t index) const {
        return (index>=x().size())?x()[x().size()-1]:x()[index];      
    }

    const std::vector<float>& height() const {
        return height_;
    }

    float height(std::size_t index) const {
        return (index>=height().size())?height()[height().size()-1]:height()[index];      
    }

    const std::vector<float>& width() const {
        return width_;
    }
 
    float width(std::size_t index) const {
        return (index>=width().size())?width()[width().size()-1]:width()[index];      
    }
    
    const std::vector<float>& bottom() const {
        return bottom_;
    }

    float bottom(std::size_t index) const {
        return (index>=bottom().size())?bottom()[bottom().size()-1]:bottom()[index];      
    }

    Bar& width(const std::vector<float>& w) {
        width_ = w; return *this;
    }
    
    Bar& width(float w) {
        return width(std::vector<float>{w});
    }

    template<typename Collection>
    Bar& width(const Collection& c) {
        return width(std::vector<float>(c));
    }
    
    Bar& bottom(const std::vector<float>& w) {
        bottom_ = w; return *this;
    }
    
    Bar& bottom(float w) {
        return bottom(std::vector<float>{w});
    }

    template<typename Collection>
    Bar& bottom(const Collection& c) {
        return bottom(std::vector<float>(c));
    }

	Bar& color(const Color& c) { color_=&c; return *this; }
	const Color& color() const { return color_?*color_:black; }
	
	std::string to_string(const _2d::Matrix& m) const noexcept override {
        auto g = _2d::group();
        for (std::size_t i = 0; i<x().size(); ++i) {
            g.add(_2d::rect({x(i)-0.5f*width(i),bottom(i)},{x(i)+0.5f*width(i),bottom(i)+height(i)})).stroke_width(0).fill(color());
        }
        return g.to_string(m);;   
	}

	std::array<float,4> axis() const override {
        std::array<float,4> ax{x(0)-0.75f*width(0),x(0)+0.75f*width(0),bottom(0)+1.1f*height(0)};
        for (std::size_t i = 1; i<x().size(); ++i) {
            if ((x(i)-0.75f*width(i))<ax[0]) ax[0] = x(i)-0.75f*width(i);
            if ((x(i)+0.75f*width(i))>ax[1]) ax[1] = x(i)+0.75f*width(i);
            if (bottom(i)<ax[2]) ax[2] = bottom(i);
            if ((bottom(i)+1.1*height(i))>ax[3]) ax[3] = bottom(i)+1.1f*height(i);
        }
        return ax;
	}

};

	
}
