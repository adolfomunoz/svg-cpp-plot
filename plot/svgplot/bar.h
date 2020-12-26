#pragma once

#include <list>
#include <tuple>
#include <string>
#include "plottable.h"
#include "../../2d/transform.h"
#include "../../2d/points.h"
#include "../../2d/polyline.h"
#include "color.h"

namespace svg_cpp_plot {

class Bar : public Plottable  {
    std::vector<float> x_;
    std::vector<float> height_;
    std::vector<float> width_;
    std::vector<float> bottom_;
    std::vector<std::shared_ptr<Color>> color_;
    float alpha_;
public:
	Bar(const std::vector<float>& x, const std::vector<float>& height) : x_(x), height_(height), width_(1,0.8f), bottom_(1,0.0f), color_(1),alpha_(1) {}

    const std::vector<float>& x() const {
        return x_;
    }

    float x(std::size_t index) const {
        return x()[index%x().size()];      
    }

    const std::vector<float>& height() const {
        return height_;
    }

    float height(std::size_t index) const {
        return height()[index%height().size()];      
    }

    const std::vector<float>& width() const {
        return width_;
    }
 
    float width(std::size_t index) const {
        return width()[index%width().size()];      
    }
    
    const std::vector<float>& bottom() const {
        return bottom_;
    }

    float bottom(std::size_t index) const {
        return bottom()[index%bottom().size()];      
    }

    Bar& width(const std::vector<float>& w) {
        width_ = w; return *this;
    }
    
    Bar& width(float w) {
        return width(std::vector<float>{w});
    }

    template<typename Collection>
    Bar& width(const Collection& c) {
        return width(std::vector<float>(c.begin(),c.end()));
    }
    
    Bar& bottom(const std::vector<float>& w) {
        bottom_ = w; return *this;
    }
    
    Bar& bottom(float w) {
        return bottom(std::vector<float>{w});
    }

    template<typename Collection>
    Bar& bottom(const Collection& c) {
        return bottom(std::vector<float>(c.begin(),c.end()));
    }
    
    Bar& alpha(float f) { alpha_=f; return *this; }
    float alpha() const { return alpha_; }


    template<typename C>
	Bar& color(const C& c) { color_=detail::color_vector(c); return *this; }
	Bar& color(const std::vector<std::string>& c) { color_=detail::color_vector(c); return *this; }
//	Bar& color(const std::vector<rgb>& c) { color_=detail::color_vector(c); return *this; }
    
	const Color& color(std::size_t index) const { 
        return color_[index % color_.size()]?*color_[index % color_.size()]:black; 
    }
	
    std::shared_ptr<_2d::Element> scaled(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept override {
        auto g = std::make_shared<_2d::Group>();
        for (std::size_t i = 0; i<x().size(); ++i) {
            g->add(_2d::rect({xscale.transform(x(i)-0.5f*width(i)),yscale.transform(bottom(i))},
                            {xscale.transform(x(i)+0.5f*width(i)),yscale.transform(bottom(i)+height(i))})).stroke_width(0).fill(color(i)).fill_opacity(alpha());
        }
        
        return g;
    }

	std::array<float,4> axis() const noexcept override {
        std::array<float,4> ax{x(0)-0.75f*width(0),x(0)+0.75f*width(0),bottom(0),bottom(0)+1.1f*height(0)};
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
