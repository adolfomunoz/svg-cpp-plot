#pragma once

#include <list>
#include <tuple>
#include <memory>
#include "svgplot/arange.h"
#include "graph-2d.h"
#include "svgplot/plot.h"
#include "svgplot/imshow.h"
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
	std::array<float,2> figsize_;
	std::vector<std::unique_ptr<Color>> cycle; std::size_t cycle_pos;

	PlotGroup plots;
	
	std::string ylabel_, xlabel_, title_;
	std::array<float,4> axis_; bool axis_set;
	
	std::vector<std::string> xticklabels_; bool xticklabels_set;
	std::vector<float> xticks_; bool xticks_set;
	std::vector<std::string> yticklabels_; bool yticklabels_set;
	std::vector<float> yticks_; bool yticks_set;
	
	
	std::unique_ptr<ImShow> imshow_;
	
	
	std::list<std::unique_ptr<SVGPlot>> subplots_;
	SVGPlot* parent; int nrows, ncols, index;
public:
	std::array<float,2> figsize() const { 
		if (parent) return std::array<float,2>{
			parent->figsize()[0]/float(nrows),
			parent->figsize()[1]/float(ncols)};	
		else return figsize_; }
	
	SVGPlot& figsize(const std::array<float,2>& v) { figsize_=v; return (*this);}

	std::string_view title() const { return title_; }
	SVGPlot& title(std::string_view l) { title_=l; return (*this); }
	void set_title(std::string_view l) { title(l); }
	std::string_view ylabel() const { return ylabel_; }
	SVGPlot& ylabel(std::string_view l) { ylabel_=l; return (*this); }
	void set_ylabel(std::string_view l) { ylabel(l); }
	std::string_view xlabel() const { return xlabel_; }
	SVGPlot& xlabel(std::string_view l) { xlabel_=l; return (*this); }
	void set_xlabel(std::string_view l) { xlabel(l); }
	
	SVGPlot& xticks(const std::vector<float>& v) {
		xticks_ = v; xticks_set=true; return (*this);
	}
	void set_xticks(const std::vector<float>& v) {
		xticks(v);
	}
	SVGPlot& yticks(const std::vector<float>& v) {
		yticks_ = v; yticks_set=true; return (*this);
	}
	void set_yticks(const std::vector<float>& v) {
		yticks(v);
	}
	SVGPlot& xticklabels(const std::vector<std::string>& v) {
		xticklabels_ = v; xticklabels_set=true; return (*this);
	}
	void set_xticklabels(const std::vector<std::string>& v) {
		xticklabels(v);
	}
	SVGPlot& yticklabels(const std::vector<std::string>& v) {
		yticklabels_ = v; yticklabels_set=true; return (*this);
	}
	void set_yticklabels(const std::vector<std::string>& v) {
		yticklabels(v);
	}

	SVGPlot& axis(const std::array<float,4> a) { axis_set=true; axis_=a; return (*this); }
	std::array<float,4> axis() const {
		if (axis_set) return axis_;
		else if (imshow_) return imshow_->axis();
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
	
	std::vector<float> xticks() const {
		if (xticks_set) return xticks_;
		else if (xticklabels_set) {
			auto [xmin,xmax,d1,d2] = axis();
			std::vector<float> sol(xticklabels_.size());
			float dx = (xmax-xmin)/xticklabels_.size();
			for (std::size_t i = 0;i<xticklabels_.size();++i)
				sol[i]=xmin + float(0.5+i)*dx;
			return sol;
		} else {
			const float target_ticks = 5;
			auto [xmin,xmax,d1,d2] = axis();
			float tick_step = std::floor((xmax - xmin)/float(target_ticks));
			int factor = 2;
			while (tick_step<=0.0f) {
				tick_step=std::floor(factor*(xmax - xmin)/float(target_ticks))/float(factor);
				if ((factor % 4) == 0) factor = (factor*10)/4;
				else factor*=2;
			}
			float first_tick = std::ceil(xmin/tick_step)*tick_step;
			std::vector<float> sol;
			for (float x = first_tick; x <= xmax; x+=tick_step)
				sol.push_back(x);
			return sol;
		}
	}
	
	std::vector<float> yticks() const {
		if (yticks_set) return yticks_;
		else if (yticklabels_set) {
			auto [d1,d2,ymin,ymax] = axis();
			std::vector<float> sol(yticklabels_.size());
			float dy = (ymax-ymin)/yticklabels_.size();
			for (std::size_t i = 0;i<yticklabels_.size();++i)
				sol[i]=ymin + float(0.5+i)*dy;
			return sol;
		} else {
			const float target_ticks = 5;
			auto [d1,d2,ymin,ymax] = axis();
			float tick_step = std::floor((ymax - ymin)/float(target_ticks));
			int factor = 2;
			while (tick_step<=0.0f) {
				tick_step=std::floor(factor*(ymax - ymin)/float(target_ticks))/float(factor);
				if ((factor % 4) == 0) factor = (factor*10)/4;
				else factor*=2;
			}
			float first_tick = std::ceil(ymin/tick_step)*tick_step;
			std::vector<float> sol;
			for (float y = first_tick; y <= ymax; y+=tick_step)
				sol.push_back(y);
			return sol;
		}
	}
	
	std::vector<std::string> xticklabels() const {
		if (xticklabels_set) return xticklabels_;
		else {
			std::vector<float> x = xticks();
			std::vector<std::string> sol(x.size());
			for (std::size_t i = 0; i<x.size(); ++i) {
				std::stringstream s; 
				s<<((x[i]==0)?0:x[i]);
				sol[i]=s.str();
			}
			return sol;
		}
	}
	
	std::vector<std::string> yticklabels() const {
		if (yticklabels_set) return yticklabels_;
		else {
			std::vector<float> y = yticks();
			std::vector<std::string> sol(y.size());
			for (std::size_t i = 0; i<y.size(); ++i) {
				std::stringstream s; 
				s<<((y[i]==0)?0:y[i]);
				sol[i]=s.str();
			}
			return sol;
		}
	}
	
private:
	void add_title(Graph2D& graph, const std::array<float,2> graph_size, std::array<float,4>& margin) const {
		if (!title().empty()) {
			graph.add(
				_2d::text({0.5*graph_size[0],-10},title()))
					.font_size(16)
					.text_anchor(svg_cpp_plot::text_anchor_middle);
			margin[2]+=30;
		}
	}

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
	
	void add_xticks(Graph2D& graph, const std::array<float,2> graph_size, std::array<float,4>& margin, const std::array<float,4>& local_axis) const {
		auto x = xticks();
		auto labels = xticklabels();
		for (std::size_t i=0;i<x.size();++i) {
			float global_x = graph_size[0]*(x[i] - local_axis[0])/
						(local_axis[1]-local_axis[0]);
			graph.add(_2d::line(
				{global_x,graph_size[1]},
				{global_x, graph_size[1]+3}))
				.stroke(black).stroke_width(1);
			if (i<labels.size()) {
				graph.add(
					_2d::text({global_x,graph_size[1]+5},labels[i]))
						.font_size(10)
						.text_anchor(text_anchor_middle)
						.dominant_baseline(dominant_baseline_hanging);
			}
		}
		if (!labels.empty()) margin[3]+=15;
		else if (!x.empty()) margin[3]+=3;
	}
	
	void add_yticks(Graph2D& graph, const std::array<float,2> graph_size, std::array<float,4>& margin, const std::array<float,4>& local_axis) const {
		auto y = yticks();
		auto labels = yticklabels();
		for (std::size_t i=0;i<y.size();++i) {
			float global_y = graph_size[1] - graph_size[1]*(y[i] - local_axis[2])/(local_axis[3]-local_axis[2]);
			graph.add(_2d::line({-3,global_y},{0,global_y}))
				.stroke(black).stroke_width(1);
			if (i<labels.size()) {		
				graph.add(_2d::text({-5,global_y},labels[i])).font_size(10).text_anchor(text_anchor_end).dominant_baseline(dominant_baseline_middle);
			}
		}
		if (!labels.empty()) margin[0]+=25;
		else if (!y.empty()) margin[0]+=3;		
	}
	
	Graph2D graph(std::array<float,4>& margin) const {
		std::array<float,2> graph_size = figsize();
		margin = std::array<float,4>{1,1,1,1};
		std::array<float,4> local_axis = axis();
		Graph2D graph({graph_size[0],graph_size[1]},BoundingBox(local_axis[0],local_axis[2],local_axis[1],local_axis[3]));
		
		for (auto p : plots) graph.area().add(p);
		if (imshow_) graph.area().add(*imshow_);
		
		add_xticks(graph, graph_size, margin, local_axis);
		add_yticks(graph, graph_size, margin, local_axis);
		add_xlabel(graph, graph_size, margin);
		add_ylabel(graph, graph_size, margin);
		add_title( graph, graph_size, margin);
		

		graph.border().stroke_width(1).stroke(black);
		return graph;
	}
public:
	Graph2D graph() const {
		std::array<float,4> margin;
		return graph(margin);
	}
protected:		
	SVG svg() const {
		std::array<float,2> graph_size = figsize();
		std::array<float,4> margin{1,1,1,1};
		SVG s;
		s.add(graph(margin));
		s.viewBox(BoundingBox(
			-margin[0],-margin[2],
			graph_size[0]+margin[1],graph_size[1]+margin[3]));
		return s;
	}
public:
	SVGPlot() :
		figsize_{200,150}, cycle_pos(0), axis_set(false),xticklabels_set(false), xticks_set(false), yticklabels_set(false), yticks_set(false), parent(nullptr) {
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
		
	ImShow& imshow(const std::vector<std::vector<float>>& data) {
		imshow_=std::make_unique<ImShow>(data);
		return *imshow_;
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