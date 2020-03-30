#pragma once

#include <list>
#include <tuple>
#include <memory>
#include "graph-2d.h"
#include "../2d/point-list.h"
#include "../2d/polyline.h"

namespace svg_cpp_plot {
		
class SVGPlot {
	class Line {
		std::shared_ptr<_2d::polyline> pl;
	public:
		Line(const std::shared_ptr<_2d::polyline>& l) : pl(l) {}
		
		Line& linewidth(int lw) { pl->stroke_width(lw); return *this; }
	};
	
	std::vector<std::unique_ptr<Color>> cycle; std::size_t cycle_pos;	
	std::list<std::shared_ptr<_2d::PointList>> plot_points;
	std::list<std::shared_ptr<_2d::Element>> plots;
	
	template<typename X, typename Y>
	Line plot_line(const X& x, const Y& y,
					const Color& c) {
		auto p = std::make_shared<_2d::polyline>();
		typename X::const_iterator ix; 
		typename Y::const_iterator iy;
		for (ix=x.begin(),iy=y.begin(); 
		    (ix!=x.end()) && (iy!=y.end());
			++ix, ++iy) p->add_point(*ix,*iy);
		
		p->stroke_linecap(stroke_linecap_round).stroke_width(1).fill(none).stroke(c);
		plots.push_back(p);
		plot_points.push_back(p);
		return Line(p);
	}
	
	SVG svg() const {
		SVG s;
		float minx(0), miny(0), maxx(0), maxy(0); bool first=true;
		for (const auto& plot : plot_points)
			for (const auto& [x,y] : plot->point_list()) {
				if (first || (minx>x)) minx=x;
				if (first || (maxx<x)) maxx=x;
				if (first || (miny>y)) miny=y;
				if (first || (maxy<y)) maxy=y;
				first = false;
			}
		
		auto& graph = s.add(Graph2D({400,300},BoundingBox(minx,miny,maxx,maxy)));
		for (const auto& plot : plots) graph.area().add(plot);
		graph.border().stroke_width(1).stroke(black);
		s.viewBox(BoundingBox(-1,-1,401,301));
		return s;
	}
public:
	SVGPlot() :
		cycle_pos(0)	{
			cycle.push_back(std::make_unique<color_hex>("1f77b4"));
			cycle.push_back(std::make_unique<color_hex>("ff7f0e"));
			cycle.push_back(std::make_unique<color_hex>("2ca02c"));
			cycle.push_back(std::make_unique<color_hex>("d62728"));
			cycle.push_back(std::make_unique<color_hex>("9467bd"));
			cycle.push_back(std::make_unique<color_hex>("8c564b"));
			cycle.push_back(std::make_unique<color_hex>("e377c2"));
			cycle.push_back(std::make_unique<color_hex>("7f7f7f"));
			cycle.push_back(std::make_unique<color_hex>("bcbd22"));
			cycle.push_back(std::make_unique<color_hex>("17becf"));
		}
		
	template<typename X, typename Y>
	void plot(const X& x, const Y& y, 
		typename std::enable_if<std::is_floating_point<typename X::value_type>::value, int>::type = 0) {
			plot_line(x,y,*cycle[cycle_pos]);
			cycle_pos = (cycle_pos + 1) % cycle.size();
	}
			
	void plot(const std::initializer_list<float>& x,
			  const std::initializer_list<float>& y) {	  
		plot(std::list<float>(x),std::list<float>(y));
	}
	
	void savefig(const std::string& name) const {
		std::ofstream f(name);
		f<<svg();
	}
};

}