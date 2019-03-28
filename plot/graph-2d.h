#pragma once

#include "../primitives/group.h"
#include "../primitives/rect.h"
#include "../attributes/style.h"
#include <cassert>

namespace svg_cpp_plot {

	
class Graph2D : public Group {
	std::tuple<float, float> size; //<-- The group's bounding box should be (0,0) to size
	BoundingBox bb; //<-- This is the bounding box in world's coordinates, that is fit into (0,0) size
	Style& style_;
	Group& area_;
	Rect& border_;
	Group& areaplots_;
	Group& plots_;

	auto xtick(float xlocal, float height, float ylocal, const std::string& classname) {
		return this->add(Line(xlocal,ylocal-0.5f*height,xlocal,ylocal+0.5f*height)).class_(classname);
	}

	auto ytick(float ylocal, float width, float xlocal, const std::string& classname) {
		return this->add(Line(xlocal-0.5f*width,ylocal,xlocal+0.5f*width,ylocal)).class_(classname);
	}



public:
	Style& graph_style() noexcept { return style_; }
	Group& area() noexcept { return area_; }
	Rect& border() noexcept { return border_; }
	//Note that if it already exists it is not added but just returned.
//	StyleEntry& all_plots() noexcept { return style().add_class("plot"); }

	Graph2D& graph_style(const Style& s) { 
		style_.merge_with(s); //Does not seem to be working...
		style_.add_id(this->id()).nest("*").vector_effect(non_scaling_stroke).nest("*").vector_effect(non_scaling_stroke).nest("*").vector_effect(non_scaling_stroke);
		return (*this);
	}

	Graph2D& default_graph_style() {
		auto& plot = graph_style().add_class("plot").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(40)).fill(none);
		plot.nth_of_type(1,7).stroke(red);
		plot.nth_of_type(2,7).stroke(yellow);
		plot.nth_of_type(3,7).stroke(pink);
		plot.nth_of_type(4,7).stroke(green);
		plot.nth_of_type(5,7).stroke(purple);
		plot.nth_of_type(6,7).stroke(orange);
		plot.nth_of_type(7,7).stroke(blue);

		graph_style().add_class("plotarea").stroke_width(0).fill(none);

		graph_style().add_class("axis").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(40)).stroke(black);
		graph_style().add_class("border").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(40)).stroke(black);
		graph_style().add_class("tick").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(80)).stroke(black);
		graph_style().add_class("label").font_size(float(std::get<1>(size))/float(20));
		style_.add_id(this->id()).nest("*").vector_effect(non_scaling_stroke).nest("*").vector_effect(non_scaling_stroke).nest("*").vector_effect(non_scaling_stroke);

		return (*this);;
	}

	template<typename F>
	Graph2D& plot_function(const F& f, float xmin, float xmax, unsigned int nsamples = 100) {
		plots_.add(svg_cpp_plot::plot_function(f,xmin,xmax,nsamples)).class_("plot");
		areaplots_.add(svg_cpp_plot::plot_function_area(f,xmin,xmax,nsamples)).class_("plotarea");
		return (*this);
	}

	template<typename F>
	Graph2D& plot_function(const F& f, unsigned int nsamples = 100) {
		return plot_function(f, std::get<0>(bb.min()), std::get<0>(bb.max()), nsamples);
	}


	Graph2D(const std::tuple<float, float>& size, const BoundingBox& bb):
	       size(size),bb(bb),style_(Group::add(Style())),area_(Group::add(Group())),border_(Group::add(Rect({0,0},size))),areaplots_(area_.add(Group())),plots_(area_.add(Group()))
	{
		border_.style().fill(none); //<-- Using local style has the highest priority
		border_.class_("border");
		area_.id("area");
		area_.transform({scale(std::get<0>(size)/bb.width(),std::get<1>(size)/bb.height()),translate(-std::get<0>(bb.min()),-std::get<1>(bb.min()))});
		area_.clip_path().add(border()).transform({translate(std::get<0>(bb.min()),std::get<1>(bb.min())),scale(bb.width()/std::get<0>(size),bb.height()/std::get<1>(size))});
		default_graph_style();
	}


	Graph2D& axis(float x = 0.0f, float y = 0.0f) {
		Group g;
		g.add(Line(x,-2.e10,x,2.e10));
		g.add(Line(-2.e10,y,2.e10,y));
		area().add(g).class_("axis");
		return (*this);
	}

	Graph2D& xticks(int count = 2, float height = 3.0f, float ylocal = 0.0f) {
		float dx = std::get<0>(size)/float(count - 1);
		for (int i = 0;i<count;++i) xtick(i*dx, height, std::get<1>(size) - ylocal, "tick");
		return (*this);
	}

	Graph2D& yticks(int count = 2, float width = 3.0f, float xlocal = 0.0f) {
		float dy = std::get<1>(size)/float(count - 1);
		for (int i = 0;i<count;++i) ytick(i*dy, width, xlocal, "tick");
		return (*this);
	}

	Graph2D& ticks(int xs = 2, int ys = 2, float size = 3.0f, float xlocal = 0.0f, float ylocal = 0.0f) {
		xticks(xs,size,ylocal);
		return yticks(ys,size,xlocal);
	}

	Graph2D& xlabels(int count = 2, float ylocal = -3.0f) {
		float dxlocal = std::get<0>(size)/float(count - 1);
		float dx = (std::get<0>(bb.max())-std::get<0>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<0>(bb.min())+i*dx);
			add(Text(i*dxlocal,std::get<1>(size) - ylocal,stext.str())).class_("label").style().text_anchor(text_anchor_middle).dominant_baseline(dominant_baseline_hanging);
		}
		return (*this);
	}

	Graph2D& ylabels(int count = 2, float xlocal = -3.0f) {
		float dylocal = std::get<1>(size)/float(count - 1);
		float dy = (std::get<1>(bb.max())-std::get<1>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<1>(bb.min())+i*dy);
			add(Text(xlocal, std::get<1>(size) - i*dylocal,stext.str())).class_("label").style().text_anchor(text_anchor_end).dominant_baseline(dominant_baseline_middle);
		}
		return (*this);
	}

};
	
};
