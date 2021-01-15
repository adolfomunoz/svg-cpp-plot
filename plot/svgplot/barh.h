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

class BarH : public Plottable  {
    std::vector<float> y_;
    std::vector<float> width_;
    std::vector<float> height_;
    std::vector<float> left_;
    std::vector<std::shared_ptr<Color>> color_;
    float alpha_;
public:
	BarH(const std::vector<float>& y, const std::vector<float>& width) : y_(y), width_(width), height_(1,0.8f), left_(1,0.0f), color_(1),alpha_(1) {}

    const std::vector<float>& y() const {
        return y_;
    }

    float y(std::size_t index) const {
        return y()[index%y().size()];      
    }

    const std::vector<float>& height() const {
        return height_;
    }
    
    const std::vector<float>& width() const {
        return width_;
    }
 
    float width(std::size_t index) const {
        return width()[index%width().size()];      
    }
    
    float height(std::size_t index) const {
        return height()[index%height().size()];      
    }

    const std::vector<float>& left() const {
        return left_;
    }

    float left(std::size_t index) const {
        return left()[index%left().size()];      
    }

    BarH& height(const std::vector<float>& h) {
        height_ = h; return *this;
    }
    
    BarH& height(float h) {
        return height(std::vector<float>{h});
    }

    template<typename Collection>
    BarH& height(const Collection& c) {
        return height(std::vector<float>(c.begin(),c.end()));
    }
    
    BarH& left(const std::vector<float>& w) {
        left_ = w; return *this;
    }
    
    BarH& left(float w) {
        return left(std::vector<float>{w});
    }

    template<typename Collection>
    BarH& left(const Collection& c) {
        return left(std::vector<float>(c.begin(),c.end()));
    }
    
    BarH& alpha(float f) { alpha_=f; return *this; }
    float alpha() const { return alpha_; }


    template<typename C>
	BarH& color(const C& c) { color_=detail::color_vector(c); return *this; }
	BarH& color(const std::vector<std::string>& c) { color_=detail::color_vector(c); return *this; }
//	Bar& color(const std::vector<rgb>& c) { color_=detail::color_vector(c); return *this; }
    
	const Color& color(std::size_t index) const { 
        return color_[index % color_.size()]?*color_[index % color_.size()]:black; 
    }
    
	
    std::shared_ptr<_2d::Element> scaled(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept override {
        auto g = std::make_shared<_2d::Group>();
        
        for (std::size_t i = 0; i<y().size(); ++i) 
            g->add(_2d::rect({xscale.transform(left(i)),yscale.transform(y(i)-0.5f*height(i))},
                            {xscale.transform(left(i)+width(i)),yscale.transform(y(i)+0.5f*height(i))})).stroke_width(0).fill(color(i)).fill_opacity(alpha());
        
        return g;
    }

private:
    std::array<float,4> axis(int i) const noexcept {
        return std::array<float,4>{left(i),left(i)+width(i),y(i)-0.5f*height(i),y(i)+0.5f*height(i)};
    }

public:
	std::array<float,4> axis() const noexcept override {
        std::array<float,4> ax = axis(0);
        for (std::size_t i = 1; i<y().size(); ++i) ax = axis_join(ax,axis(i));
        return ax;
	}    

    std::array<float,4> scaled_axis(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept override {
        std::array<float,4> ax = axis_transform(axis(0),xscale,yscale);
        for (std::size_t i = 1; i<y().size(); ++i) if (height(i)>0) ax = axis_join(ax,axis_transform(axis(i),xscale,yscale));
        return ax;
	}
};
	
}
