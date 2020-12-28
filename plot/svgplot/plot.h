#pragma once

#include <list>
#include <tuple>
#include <string>
#include "plottable.h"
#include "../../2d/transform.h"
#include "../../2d/polyline.h"
#include "scatter.h"

namespace svg_cpp_plot {

class Plot : public Plottable  {
    std::list<std::tuple<float,float>> data;
	std::string format_; 
	std::shared_ptr<Color> color_;
	float linewidth_;
	float markersize_;
    float alpha_;
public:
	template<typename X, typename Y>
	Plot(const X& x, const Y& y) : color_(nullptr),linewidth_(1),markersize_(1),alpha_(1) {
		auto ix = x.begin(); auto iy = y.begin();
		for (;(ix != x.end()) && (iy != y.end());++ix,++iy)
			data.push_back({*ix,*iy});
	}
	
	Plot& format(std::string_view f) { format_=f; return *this; }
	std::string_view format() const { return format_; }
	Plot& linestyle(std::string_view f) { return format(f); }
	std::string_view linestyle() const { return format(); }
	Plot& marker(std::string_view f) { return format(f); }
	std::string_view marker() const { return format(); }

	Plot& linewidth(float f) { linewidth_=f; return *this; }
	float linewidth() const { return linewidth_; }
	Plot& markersize(float f) { markersize_=f; return *this; }
	float markersize() const { return markersize_; }
    Plot& alpha(float f) { alpha_=f; return *this; }
    float alpha() const { return alpha_; }

    template<typename C>
	Plot& color(const C& c) { color_=detail::color(c); return *this; }
	const Color& color() const { return color_?*color_:black; }
    
    std::shared_ptr<_2d::Element> scaled(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept override {
        if (format().empty() || (format()[0] == '-') || (format()[0] == ':')) { // This is a line, not a point that can be represented by a scatter plot
            auto pl = std::make_shared<_2d::polyline>();
            for (auto [x,y] : data) 
                if (xscale.is_valid(x) && yscale.is_valid(y))
                    pl->add_point(xscale.transform(x),yscale.transform(y));
                
            
			pl->stroke_width(this->linewidth()).stroke(this->color()).stroke_linecap(stroke_linecap_round).stroke_opacity(alpha());
			if (format() == "--") pl->stroke_dasharray({3,3});
			else if	(format() == "-.") pl->stroke_dasharray({3,2,1,2});
			else if (format()==":") pl->stroke_dasharray({1,2});
            return pl;
        } else { // The format indicates that it is drawn with points so we use a scatter plot for plotting this
            return Scatter(std::vector<std::tuple<float,float>>(data.begin(),data.end())).marker(format()).s(markersize()).c(color_).alpha(alpha()).scaled(xscale,yscale);
        }      
    }

    
    std::array<float,4> axis() const noexcept override {
        std::array<float,4> ax{std::get<0>(data.front()),std::get<0>(data.front()),std::get<1>(data.front()),std::get<1>(data.front())};
        for (auto [x,y] : data) {
            if (x < ax[0]) ax[0] = x;
            if (x > ax[1]) ax[1] = x;
            if (y < ax[2]) ax[2] = y;
            if (y > ax[3]) ax[3] = y;
        }
		float dx = (ax[1]-ax[0])/32.0f;
		float dy = (ax[3]-ax[2])/32.0f;
        ax[0]-=dx; ax[1]+=dx; ax[2]-=dy; ax[3]+=dy;

        return ax;
	}
};

/**
 * Save this as an idea to implement into plottable but not use it right now. We will see.
 */
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
    template<typename C>
	PlotGroup& color(const C& c) {
		for (Plot& p : (*this)) p.color(c);  
		return *this; 
	}
		
	Plot& add(const Plot& plot) {
		this->push_back(plot); 
		return this->back();
	}
};
	
}