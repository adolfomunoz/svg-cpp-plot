#pragma once

#include <list>
#include <tuple>
#include <string>
#include "../../2d/transform.h"
#include "../../2d/points.h"
#include "../../2d/polyline.h"

namespace svg_cpp_plot {

class Plot : public std::list<std::tuple<float,float>>, public Generator<_2d::Matrix>  {
	std::string format_; 
	const Color* color_;
	float linewidth_;
	float markersize_;
public:
	template<typename X, typename Y>
	Plot(const X& x, const Y& y) : color_(nullptr),linewidth_(1),markersize_(1) {
		auto ix = x.begin(); auto iy = y.begin();
		for (;(ix != x.end()) && (iy != y.end());++ix,++iy)
			this->push_back({*ix,*iy});
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

	Plot& color(const Color& c) { color_=&c; return *this; }
	const Color& color() const { return color_?*color_:black; }
	
	std::string to_string(const _2d::Matrix& m) const noexcept override {
		if (format() == "o")
			return _2d::points(*this).
				set_symbol(_2d::circle({0,0},1.2*markersize()).stroke_width(0).fill(color())).to_string(m);
		else if (format() == ".")
			return _2d::points(*this).
				set_symbol(_2d::circle({0,0},0.6*markersize()).stroke_width(0).fill(color())).to_string(m);
		else if (format() == ",")
			return _2d::points(*this).
				set_symbol(_2d::circle({0,0},0.2*markersize()).stroke_width(0).fill(color())).to_string(m);
		else if (format() == "v")
			return _2d::points(*this).
				set_symbol(_2d::triangle({0,1*markersize()},{1*markersize(),-1*markersize()},{-1*markersize(),-1*markersize()}).stroke_width(0).fill(color())).to_string(m);
		else if (format() == "^")
			return _2d::points(*this).
				set_symbol(_2d::triangle({0,-1*markersize()},{1*markersize(),1*markersize()},{-1*markersize(),1*markersize()}).stroke_width(0).fill(color())).to_string(m);
		else if (format() == "s")
			return _2d::points(*this).
				set_symbol(_2d::rect({-1*markersize(),-1*markersize()},{1*markersize(),1*markersize()}).stroke_width(0).fill(color())).to_string(m);
		else if (format() == "+")
			return _2d::points(*this).
				set_symbol(_2d::plus({0,0},2*markersize()).stroke_width(0.5*markersize()).stroke(color())).to_string(m);
		else if (format() == "P")
			return _2d::points(*this).
				set_symbol(_2d::plus({0,0},2*markersize()).stroke_width(1*markersize()).stroke(color())).to_string(m);
		else if (format() == "x")
			return _2d::points(*this).
				set_symbol(_2d::times({0,0},2*markersize()).stroke_width(0.5*markersize()).stroke(color())).to_string(m);
		else if (format() == "X")
			return _2d::points(*this).
				set_symbol(_2d::times({0,0},2*markersize()).stroke_width(1*markersize()).stroke(color())).to_string(m);
		else {
			auto pl = _2d::polyline(*this).stroke_width(this->linewidth()).stroke(this->color()).stroke_linecap(stroke_linecap_round);
			if (format() == "--") pl.stroke_dasharray({3,3});
			else if	(format() == "-.") pl.stroke_dasharray({3,2,1,2});
			else if (format()==":") pl.stroke_dasharray({1,2});
			
			return pl.to_string(m);
		}
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