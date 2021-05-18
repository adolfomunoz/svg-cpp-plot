#pragma once

#include <list>
#include <tuple>
#include <string>
#include "plottable.h"
#include "../../2d/transform.h"
#include "../../2d/points.h"
#include "../../2d/ignore-scale.h"
#include "../../2d/polyline.h"
#include "../../2d/polygon.h"
#include "color.h"
#include "cmap.h"

namespace svg_cpp_plot {
    
using _2d::operator*;
    
class ScatterColor {
    std::string cmap_;
	float vmin_, vmax_; bool vmin_set, vmax_set;

protected:
    virtual float calculated_vmin() const = 0;
    virtual float calculated_vmax() const = 0;

public:   
    ScatterColor() : vmin_set(false), vmax_set(false) {}
	ScatterColor& vmin(float f)  { vmin_=f; vmin_set=true; return (*this);}
	ScatterColor& vmax(float f)  { vmax_=f; vmax_set=true; return (*this);}

 	float vmin() const {
		if (vmin_set) return vmin_;
        else return calculated_vmin();
    }
    
 	float vmax() const {
		if (vmax_set) return vmax_;
        else return calculated_vmax();
    }
   
	ScatterColor& cmap(std::string_view s) { cmap_=s; return *this; }
	std::string_view cmap() const { return cmap_; }
    
    virtual std::shared_ptr<Color> color(int i) const = 0;
    virtual float opacity(int i) const = 0;
};

class ScatterColorConstant : public ScatterColor {
    std::shared_ptr<Color> color_; 
public:
    ScatterColorConstant(const std::shared_ptr<Color>& color) :
        color_(color) { }
    float calculated_vmin() const override { return 0; }
    float calculated_vmax() const override { return 1; }
    std::shared_ptr<Color> color(int i) const override { return color_; }
    float opacity(int i) const override { return 1.0f; }
};

namespace {
inline _2d::polygon plus(float s, float w) {
    const float hs = 0.5f*s;
    const float hw = 0.5f*w;
    return _2d::polygon({{hw,hw},{hs,hw},{hs,-hw},{hw,-hw},{hw,-hs},{-hw,-hs},{-hw,-hw},{-hs,-hw},{-hs,hw},{-hw,hw},{-hw,hs},{hw,hs}});
}
inline _2d::Group times(float s, float w) {
    auto g = _2d::group(_2d::rotate(0.25*M_PI));
    g.add(plus(s*sqrt(2),w));
    return g;
}
}

template<typename T>
class ScatterColorType : public ScatterColor {
    std::vector<T> data;

public:
    ScatterColorType(const std::vector<T>& data) : data(data) {}
    ScatterColorType(std::vector<T>&& data) : data(std::forward<std::vector<T>>(data)) {}
    template<typename Collection>
    ScatterColorType(const Collection& d) : ScatterColorType(std::vector<T>(d.begin(),d.end())) {}
protected:
    float calculated_vmin() const override {
        float temp_vmin = detail::vmin_element(data[0]);
        for (const auto& value : data) {
            float v = detail::vmin_element(value);
            if (v < temp_vmin) temp_vmin = v;
        }
        return temp_vmin;       
    }
    
    float calculated_vmax() const override {
        float temp_vmax = detail::vmax_element(data[0]);
        for (const auto& value : data) {
            float v = detail::vmax_element(value);
            if (v > temp_vmax) temp_vmax = v;
        }
        return temp_vmax;         
    }
public:
    std::shared_ptr<Color> color(int i) const override {
        auto [r,g,b] = detail::color_of(data[i%data.size()],detail::colormap(cmap(),vmin(),vmax()));
        return std::make_shared<rgb>(r,g,b);
    }
    
    float opacity(int i) const override {
        return detail::opacity_of(data[i%data.size()],detail::colormap(cmap(),vmin(),vmax()));
    }
};

class Scatter : public Plottable  {
    std::vector<std::tuple<float,float>> data;
    std::unique_ptr<ScatterColor> scatter_color, edgecolors_;
    std::vector<float> markersize_, linewidths_;
    std::shared_ptr<_2d::Element> marker_;
//    bool border_based_marker_; <- We cannot have border_based_markers if we have to scale them so it is better that we have a single type of marker that just takes us a little bit longer to define
    float alpha_;
   
    
public:
    template<typename T>
	Scatter& marker(const T& t,
		std::enable_if_t<std::is_base_of_v<_2d::Element,T>,int> dummy = 0) {
		marker_ = std::make_shared<T>(t);
		return (*this);
	}
	
	Scatter& marker(const std::shared_ptr<_2d::Element>& t) {
		marker_ = t;
		return (*this);
	}

	Scatter& marker(std::string_view f) { 
		if (f == "o") return marker(_2d::circle({0,0},1));
        else if (f == ".") return marker(_2d::circle({0,0},0.5));
		else if (f == ",") return marker(_2d::circle({0,0},0.23));
		else if (f == "v") return marker(_2d::triangle({0,1},{1,-1},{-1,-1}));
		else if (f == ">") return marker(_2d::triangle({1,0},{-1,1},{-1,-1}));
		else if (f == "^") return marker(_2d::triangle({0,-1},{1,1},{-1,1}));
		else if (f == "<") return marker(_2d::triangle({-1,0},{1,1},{1,-1}));
		else if (f == "s") return marker(_2d::rect({-1,-1},{1,1}));
        //From now on size, color and alpha affect stroke but not fill. We setup the stroke width here
        else if (f == "+") return marker(plus(2,0.3));
 		else if (f == "P") return marker(plus(2,0.7)); 
		else if (f == "x") return marker(times(2,0.3));
		else if (f == "X") return marker(times(1.5,0.7));
		else return marker(_2d::circle({0,0},1.2)); //By default, a circle
    
        return *this; 
    }
     
    Scatter(const std::vector<std::tuple<float,float>>& data) : data(data), scatter_color(std::make_unique<ScatterColorConstant>(std::make_shared<blackColor>())),edgecolors_(std::make_unique<ScatterColorConstant>(std::make_shared<blackColor>())),markersize_(1,1.0f),linewidths_(1,0.0f),alpha_(1) { marker("o"); }
   
	template<typename X, typename Y>
	Scatter(const X& x, const Y& y) : scatter_color(std::make_unique<ScatterColorConstant>(std::make_shared<blackColor>())),edgecolors_(std::make_unique<ScatterColorConstant>(std::make_shared<blackColor>())),markersize_(1,1.0f),linewidths_(1,0.0f),alpha_(1) {
        marker("o");
		auto ix = x.begin(); auto iy = y.begin();
		for (;(ix != x.end()) && (iy != y.end());++ix,++iy)
			data.push_back({*ix,*iy});
	}


    Scatter& vmin(float f) { scatter_color->vmin(f); return *this; }
    Scatter& vmax(float f) { scatter_color->vmax(f); return *this; }
    Scatter& cmap(std::string_view s) { scatter_color->cmap(s); return *this; }

	Scatter& s(float f) { markersize_=std::vector<float>(1,f); return *this; }
	Scatter& s(const std::vector<float>& f) { markersize_=f; return *this; }
	Scatter& s(std::vector<float>&& f) { markersize_=std::move(f); return *this; }
    
    Scatter& linewidths(float f) { linewidths_=std::vector<float>(1,f); return *this; }
    Scatter& linewidths(const std::vector<float>& f) { linewidths_=f; return *this; }
    Scatter& linewidths(std::vector<float>&& f) { linewidths_=std::move(f); return *this; }
    
    
    template<typename C>
	Scatter& c(const C& col, typename std::enable_if<!std::is_base_of<Color,C>::value,int>::type x = 0) { scatter_color = std::make_unique<ScatterColorType<typename std::decay<C>::type::value_type>>(col); return *this; }
    template<typename C>
	Scatter& c(const C& col, typename std::enable_if<std::is_base_of<Color,C>::value,int>::type x = 0) { 
        scatter_color=std::make_unique<ScatterColorConstant>(std::make_shared<C>(col)); 
        return *this; 
    }
    template<typename C>
	Scatter& c(std::vector<C>&& col) { scatter_color = std::make_unique<ScatterColorType<C>>(std::forward<std::vector<C>>(col)); return *this; }
    Scatter& c(const std::initializer_list<float>& l) { return c(std::vector<float>(l)); }
    Scatter& c(const std::initializer_list<std::tuple<float,float,float>>& l) { return c(std::vector<std::tuple<float,float,float>>(l)); }
    Scatter& c(const std::initializer_list<std::tuple<float,float,float,float>>& l) { return c(std::vector<std::tuple<float,float,float,float>>(l)); }
	Scatter& c(const std::string& col) { scatter_color=std::make_unique<ScatterColorConstant>(detail::color(col)); return *this; }
	Scatter& c(const char* col) { return c(std::string(col)); return *this; }
	Scatter& c(const std::shared_ptr<Color>& col) { 
        if (col) scatter_color=std::make_unique<ScatterColorConstant>(col); 
        return *this; 
    }
    
    template<typename C>
	Scatter& edgecolors(const C& col, typename std::enable_if<!std::is_base_of<Color,C>::value,int>::type x = 0) { edgecolors_ = std::make_unique<ScatterColorType<typename std::decay<C>::type::value_type>>(col); return *this; }
    template<typename C>
	Scatter& edgecolors(const C& col, typename std::enable_if<std::is_base_of<Color,C>::value,int>::type x = 0) { 
        edgecolors_=std::make_unique<ScatterColorConstant>(std::make_shared<C>(col)); 
        return *this; 
    }
    template<typename C>
	Scatter& edgecolors(std::vector<C>&& col) { edgecolors_ = std::make_unique<ScatterColorType<C>>(std::forward<std::vector<C>>(col)); return *this; }
    Scatter& edgecolors(const std::initializer_list<float>& l) { return edgecolors(std::vector<float>(l)); }
    Scatter& edgecolors(const std::initializer_list<std::tuple<float,float,float>>& l) { return edgecolors(std::vector<std::tuple<float,float,float>>(l)); }
    Scatter& edgecolors(const std::initializer_list<std::tuple<float,float,float,float>>& l) { return edgecolors(std::vector<std::tuple<float,float,float,float>>(l)); }
	Scatter& edgecolors(const std::string& col) { edgecolors_=std::make_unique<ScatterColorConstant>(detail::color(col)); return *this; }
	Scatter& edgecolors(const char* col) { return edgecolors(std::string(col)); return *this; }
	Scatter& edgecolors(const std::shared_ptr<Color>& col) { 
        if (col) edgecolors_=std::make_unique<ScatterColorConstant>(col); 
        return *this; 
    }
    
   
private:
	float markersize(int i) const { 
        return markersize_[i%markersize_.size()]; 
    }
	float linewidth(int i) const { 
        return linewidths_[i%linewidths_.size()]; 
    }
    
public:
    Scatter& alpha(float f) { alpha_=f; return *this; }
    float alpha() const { return alpha_; }
    
    std::shared_ptr<_2d::Element> scaled(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept override {
        //Maybe this could be sped up with marker beeing a referenzable svg element but that part is not ready yet in the core primitives
		auto g = std::make_shared<_2d::Group>();
         if (data.empty()) return g;
       
        //We need this in memory in case we are generating the corresponding color and it gets out of memory
        std::vector<std::shared_ptr<Color>> colors(data.size()); 
		for (std::size_t i = 0; i<data.size(); ++i) { colors[i] = scatter_color->color(i); }
        std::vector<std::shared_ptr<Color>> edgecolors(data.size()); 
		for (std::size_t i = 0; i<data.size(); ++i) { edgecolors[i] = edgecolors_->color(i); }
        
		for (std::size_t i = 0; i<data.size(); ++i) {
            float size = markersize(i);
            auto datapoint = std::make_shared<_2d::Group>(_2d::scale({size,size}));
            datapoint->stroke_width(linewidth(i)).stroke(*edgecolors[i]).fill(*colors[i]).opacity(alpha()*scatter_color->opacity(i));
            datapoint->add_ptr(marker_);
			g->add(
                _2d::group(_2d::translate(xscale.transform(std::get<0>(data[i])),yscale.transform(std::get<1>(data[i])))))
                    .add(_2d::ignore_scale(datapoint));
		}
		return g;        
    }
   
    std::array<float,4> axis() const noexcept override {
        if (data.empty()) return std::array<float,4>{0,0,0,0};
        std::array<float,4> ax{std::get<0>(data.front()),std::get<0>(data.front()),std::get<1>(data.front()),std::get<1>(data.front())};
//        float max_size = markersize(0);
        for (std::size_t i = 0; i<data.size(); ++i) {
            auto [x,y] = data[i];
            if (x < ax[0]) ax[0] = x;
            if (x > ax[1]) ax[1] = x;
            if (y < ax[2]) ax[2] = y;
            if (y > ax[3]) ax[3] = y;
//            if (markersize(i) > max_size) max_size = markersize(i);
        }

        //Arbitrary expansion, maybe not a good idea but it is worse to account for markersize which is in a different space
		float dx = std::abs(ax[1]-ax[0])/32.0f; //+ max_size/std::abs(ax[1]-ax[0]);
		float dy = std::abs(ax[3]-ax[2])/32.0f; //+ max_size/std::abs(ax[3]-ax[2]);
        ax[0]-=dx; ax[1]+=dx; ax[2]-=dy; ax[3]+=dy;


        return ax;
	}
    
    std::array<float,4> scaled_axis(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept override {
        std::array<float,4> ax{0,0,0,0};
        if (data.empty()) return ax;
        
        bool first = true; //float max_size=0;
        for (std::size_t i = 0; i<data.size(); ++i) {
            auto [x,y] = data[i]; 
            if (xscale.is_valid(x) && yscale.is_valid(y)) {
//                float size = markersize(i);
//                if (size > max_size) max_size = size;
                float tx = xscale.transform(x); float ty = yscale.transform(y);
                if (first || ((tx) < ax[0])) ax[0] = tx;
                if (first || ((tx) > ax[1])) ax[1] = tx;
                if (first || ((ty) < ax[2])) ax[2] = ty;
                if (first || ((ty) > ax[3])) ax[3] = ty;
                first = false;
            }
        } 

        //Arbitrary expansion, maybe not a good idea but it is worse to account for markersize which is in a different space
		float dx = std::abs(ax[1]-ax[0])/32.0f; //+ max_size/std::abs(ax[1]-ax[0]);
		float dy = std::abs(ax[3]-ax[2])/32.0f; //+ max_size/std::abs(ax[3]-ax[2]);
        ax[0]-=dx; ax[1]+=dx; ax[2]-=dy; ax[3]+=dy;

        return ax;
	}
};
	
}