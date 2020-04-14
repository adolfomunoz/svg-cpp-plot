#pragma once

#include <list>
#include <tuple>
#include <memory>
#include "svgplot/arange.h"
#include "graph-2d.h"
#include "svgplot/plot.h"
#include "../2d/point-list.h"
#include "../2d/polyline.h"

namespace svg_cpp_plot {
	
namespace { //Anonymous namespace, only visible from this .h
	template<std::size_t N>
	std::array<float,N>& operator+=(std::array<float,N>& a, const std::array<float,N>& b) {
		for (std::size_t i = 0; i<N; ++i) a[i]+=b[i];
		return a;
	}
}
	

		
class SVGPlot {
	std::vector<std::unique_ptr<Color>> cycle; std::size_t cycle_pos;

	PlotGroup plots;
	
	std::string ylabel_, xlabel_;
	std::array<float,4> axis_; bool axis_set;
public:
	std::string_view ylabel() const { return ylabel_; }
	void ylabel(std::string_view l) { ylabel_=l; }
	std::string_view xlabel() const { return xlabel_; }
	void xlabel(std::string_view l) { xlabel_=l; }

	void axis(const std::array<float,4> a) { axis_=a; }
	std::array<float,4> axis() const {
		if (axis_set) return axis_;
		else {
			float minx(0), miny(0), maxx(0), maxy(0); bool first=true;
			for (const auto& plot : plots)
				for (const auto& [x,y] : plot) {
					if (first || (minx>x)) minx=x;
					if (first || (maxx<x)) maxx=x;
					if (first || (miny>y)) miny=y;
					if (first || (maxy<y)) maxy=y;
					first = false;
				}
		
			float dx = (maxx-minx)/32.0f;
			float dy = (maxy-miny)/32.0f;
			return std::array<float,4>{minx-dx,maxx+dx,miny-dy,maxy+dy};
		}
	}
	
private:
	void add_xlabel(Graph2D& graph, const std::array<float,2> graph_size, std::array<float,4>& margin) const {
		if (!xlabel().empty()) {
			graph.add(
				_2d::text({0.5*graph_size[0],graph_size[1]+26},xlabel()))
					.font_size(14)
					.text_anchor(svg_cpp_plot::text_anchor_middle);
			margin[3]+=30;
		}
	}
	
	void add_ylabel(Graph2D& graph, const std::array<float,2> graph_size, std::array<float,4>& margin) const {
		if (!ylabel().empty()) {
			graph.add(_2d::group(_2d::translate({-26,0.5*graph_size[1]})*_2d::rotate(-0.5*M_PI))).add(_2d::text({0,0},ylabel())).font_size(14).text_anchor(text_anchor_middle);
			margin[0]+=40;
		}
	}
	
	void add_xticks(Graph2D& graph, const std::array<float,2> graph_size, std::array<float,4>& margin, const std::array<float,4>& local_axis, float target_ticks) const {
		//Hacemos los ticks buscando un redondeo molón (X)
		float tick_step = std::floor((local_axis[1] - local_axis[0])/float(target_ticks));
		int factor = 2;
		while (tick_step<=0.0f) {
			tick_step=std::floor(factor*(local_axis[1] - local_axis[0])/float(target_ticks))/float(factor);
			if ((factor % 4) == 0) factor = (factor*10)/4;
			else factor*=2;
		}
		float first_tick = std::ceil(local_axis[0]/tick_step)*tick_step;
		for (float x = first_tick; x <= local_axis[1]; x+=tick_step) {
			float global_x = graph_size[0]*(x - local_axis[0])/
						(local_axis[1]-local_axis[0]);
			graph.add(_2d::line({global_x,graph_size[1]},{global_x, graph_size[1]+3}))
				.stroke(black).stroke_width(1);
						
			std::stringstream label_x; label_x<<((x==0)?0:x);
			graph.add(_2d::text({global_x,graph_size[1]+5},label_x.str())).font_size(10).text_anchor(text_anchor_middle).dominant_baseline(dominant_baseline_hanging);
		}
		margin[3]+=15;		
	}
	
	void add_yticks(Graph2D& graph, const std::array<float,2> graph_size, std::array<float,4>& margin, const std::array<float,4>& local_axis, float target_ticks) const {
		//Hacemos los ticks buscando un redondeo molón (X)
		float tick_step = std::floor((local_axis[3] - local_axis[2])/float(target_ticks));
		int factor = 2;
		while (tick_step<=0.0f) {
			tick_step=std::floor(factor*(local_axis[3] - local_axis[2])/float(target_ticks))/float(factor);
			if ((factor % 4) == 0) factor = (factor*10)/4;
			else factor*=2;
		}
		float first_tick = std::ceil(local_axis[2]/tick_step)*tick_step;
		for (float y = first_tick; y <= local_axis[3]; y+=tick_step) {
			float global_y = graph_size[1] - graph_size[1]*(y - local_axis[2])/
						(local_axis[3]-local_axis[2]);
			graph.add(_2d::line({-3,global_y},{0,global_y}))
				.stroke(black).stroke_width(1);
						
			std::stringstream label_y; label_y<<((y==0)?0:y);
			graph.add(_2d::text({-5,global_y},label_y.str())).font_size(10).text_anchor(text_anchor_end).dominant_baseline(dominant_baseline_middle);
		}
		margin[0]+=25;	
	}
	
	
	SVG svg() const {
		std::array<float,2> graph_size{200,150};
		std::array<float,4> margin{1,1,1,1};
		
		SVG s;
		std::array<float,4> local_axis = axis();

		auto& graph = s.add(Graph2D({graph_size[0],graph_size[1]},BoundingBox(local_axis[0],local_axis[2],local_axis[1],local_axis[3])));
		for (auto p : plots) graph.area().add(p);
		
		add_xticks(graph, graph_size, margin, local_axis, 5);
		add_yticks(graph, graph_size, margin, local_axis, 5);
		add_xlabel(graph, graph_size, margin);
		add_ylabel(graph, graph_size, margin);
		

		graph.border().stroke_width(1).stroke(black);
		s.viewBox(BoundingBox(
			-margin[0],-margin[2],
			graph_size[0]+margin[1],graph_size[1]+margin[3]));
		return s;
	}
public:
	SVGPlot() :
		cycle_pos(0), axis_set(false)	{
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
	Plot& plot(const X& x, const Y& y, std::string_view fmt = "",
		typename std::enable_if<std::is_floating_point<typename X::value_type>::value && std::is_floating_point<typename Y::value_type>::value, int>::type = 0) {
			Color* color = nullptr;
			std::string_view dashes = fmt;
			if (fmt.size()>0) {
				dashes = fmt.substr(1);
				switch (fmt[0]) {
					case 'r': color = &red;     break;
					case 'g': color = &green;   break;
					case 'b': color = &blue;    break;
					case 'k': color = &black;   break;
					case 'w': color = &white;   break;
					case 'c': color = &cyan;    break;
					case 'm': color = &magenta; break;
					case 'y': color = &yellow;  break;
					default: dashes = fmt;
				}
			}
			if (!color) {
				color = cycle[cycle_pos].get();
				cycle_pos = (cycle_pos + 1) % cycle.size();
			}
			return plots.add(Plot(x,y)).color(*color).format(dashes);
	}
	
	template<typename X, typename Y>
	Plot&  plot(const X& x, const Y& y, std::string_view fmt = "",
		typename std::enable_if<std::is_floating_point<typename X::value_type>::value && std::is_floating_point<decltype(std::declval<Y>()(0.0f))>::value, int>::type = 0) {
		
		std::list<float> l;
		for (float ix : x) l.push_back(y(ix));
		return plot(x,l,fmt);
	}
	
	template<typename X>
	Plot&  plot(const X& x, const std::initializer_list<float>& y, std::string_view fmt = "") {
		return plot(x,std::list<float>(y), fmt);
	}
	
	template<typename Y>
	Plot&  plot(const std::initializer_list<float>& x,
			  const Y& y, std::string_view fmt = "") {
		return plot(std::list<float>(x),y, fmt);
	}
	
	Plot&  plot(const std::initializer_list<float>& x,
			  const std::initializer_list<float>& y, std::string_view fmt = "") {	  
		return plot(std::list<float>(x),std::list<float>(y),fmt);
	}
	
	template<typename Y>
	Plot&  plot(const Y& y, std::string_view fmt  = "") {	  
		return plot(arange(0,y.size(),1),y,fmt);
	}
	
	Plot&  plot(const std::initializer_list<float>& y, std::string_view fmt  = "") {	  
		return plot(arange(0,y.size(),1),std::list<float>(y),fmt);
	}
	
	void savefig(const std::string& name) const {
		std::ofstream f(name);
		f<<svg();
	}	
};

}