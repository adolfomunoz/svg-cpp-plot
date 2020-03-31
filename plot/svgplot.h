#pragma once

#include <list>
#include <tuple>
#include <memory>
#include "graph-2d.h"
#include "../2d/point-list.h"
#include "../2d/polyline.h"

namespace svg_cpp_plot {
	
class arange {
	float start, stop, step;
	
public:
	arange(float start, float stop, float step) :
		start(start), stop(stop), step(step) {}
	
	using value_type=float;
	
	class const_iterator : public std::iterator<std::output_iterator_tag, int>{
		friend class arange;
		float x, step;
		const_iterator(float x , float step) :
			x(x), step(step) {}

	public:
		const_iterator() : const_iterator(0,0) {}

		float operator*() const { return x; }
		const_iterator& operator++() {
			x+=step; return (*this);
		}
		const_iterator operator++(int) {
			const_iterator i = (*this); ++(*this); return i;
		}
		bool operator==(const const_iterator& that) const {
			return (that.x > (x-0.5f*step)) && (that.x < (x+0.5f*step));
		}
		bool operator!=(const const_iterator& that) const {
			return !((*this)==that);
		}
	};
	
	const_iterator begin() const { return const_iterator(start,step); }
	const_iterator end() const { return const_iterator(stop,step); }
	
};
		
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
	void plot(const X& x, const Y& y, std::string_view fmt = "",
		typename std::enable_if<std::is_floating_point<typename X::value_type>::value && std::is_floating_point<typename Y::value_type>::value, int>::type = 0) {
			
			if (fmt.size()>0) {
				switch (fmt[0]) {
					case 'r': plot_line(x,y,red); break;
					case 'g': plot_line(x,y,green); break;
					case 'b': plot_line(x,y,blue); break;
					case 'k': plot_line(x,y,black); break;
					case 'w': plot_line(x,y,white); break;
					case 'c': plot_line(x,y,rgb(0,1,1)); break;
					case 'm': plot_line(x,y,rgb(1,0,1)); break;
					case 'y': plot_line(x,y,yellow); break;
					default: plot_line(x,y,*cycle[cycle_pos]);
				}
			} else plot_line(x,y,*cycle[cycle_pos]);
			cycle_pos = (cycle_pos + 1) % cycle.size();
	}
	
	template<typename X>
	void plot(const X& x, const std::initializer_list<float>& y, std::string_view fmt = "") {
			return plot(x,std::list<float>(y), fmt);
	}
	
	template<typename Y>
	void plot(const std::initializer_list<float>& x,
			  const Y& y, std::string_view fmt = "") {
		plot(std::list<float>(x),y, fmt);
	}	
	void plot(const std::initializer_list<float>& x,
			  const std::initializer_list<float>& y, std::string_view fmt = "") {	  
		plot(std::list<float>(x),std::list<float>(y),fmt);
	}
	
	template<typename Y>
	void plot(const Y& y, std::string_view fmt  = "") {	  
		plot(arange(0,y.size(),1),y,fmt);
	}
	
	void plot(const std::initializer_list<float>& y, std::string_view fmt  = "") {	  
		plot(arange(0,y.size(),1),std::list<float>(y),fmt);
	}
	
	void savefig(const std::string& name) const {
		std::ofstream f(name);
		f<<svg();
	}
};

}