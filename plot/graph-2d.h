#pragma once

#include "../primitives/group.h"
#include "../primitives/rect.h"
#include "../attributes/style.h"
#include "plot-points.h"
#include "graph-style.h"
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
	Group& points_;
	Rect& background_;

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

	Graph2D& graph_style(const GraphStyle& s) {
		this->remove_if([] (const auto& e) { return e.tag()=="style"; });
		style_ = this->add(s.style(this->size));
		style_.add_id(this->id()).nest("*").vector_effect(non_scaling_stroke).nest("*").vector_effect(non_scaling_stroke).nest("*").vector_effect(non_scaling_stroke);
		return (*this);
	}

	Path& add_plot(const Path& p, bool include_area = true) {
		Path& s = plots_.add(p).class_("plot");
		if (include_area) {
			Path a = p;
			a.vertical_line_to(0).horizontal_line_to(std::get<0>(p.first_point())).close();
			areaplots_.add(a).class_("plotarea");
		} else {
			areaplots_.add(Path(0,0)).stroke_width(0).fill(none);
		}
		return s;
	}

	Points& add_points(const Points& g) {
		return static_cast<Points&>(points_.add(g).class_("points"));
	}

	Graph2D(const std::tuple<float, float>& size, const BoundingBox& bb):
	       size(size),bb(bb),style_(Group::add(Style())),area_(Group::add(Group())),border_(Group::add(Rect({0,0},size))),areaplots_(area_.add(Group())),plots_(area_.add(Group())),points_(area_.add(Group())), background_(area_.add(Rect(-2.e10,-2.e10,2.e10,2.e10)))
	{
		border_.style().fill(none); //<-- Using local style has the highest priority
		border_.class_("border");
		area_.class_("area").style().pointer_events(pointer_events_all);  //<-- Using local style has the highest priority
		background_.class_("background").style().pointer_events(pointer_events_all);  //<-- Using local style has the highest priority
		area_.transform({scale(std::get<0>(size)/bb.width(),-std::get<1>(size)/bb.height()),translate(-std::get<0>(bb.min()),-std::get<1>(bb.min()))});
		area_.clip_path().add(border()).transform({translate(std::get<0>(bb.min()),std::get<1>(bb.min())),scale(bb.width()/std::get<0>(size),bb.height()/std::get<1>(size))});
		
		graph_style(svg_cpp_plot::graph_style::Default());
	}


	Graph2D& axis(float x = 0.0f, float y = 0.0f) {
		area().add(Line(x,-2.e10,x,2.e10)).class_("axis");
		area().add(Line(-2.e10,y,2.e10,y)).class_("axis");
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
			add(Text(i*dxlocal,std::get<1>(size) - ylocal,stext.str())).class_({"label","xlabel"}).style().text_anchor(text_anchor_middle).dominant_baseline(dominant_baseline_hanging);
		}
		return (*this);
	}

	Graph2D& ylabels(int count = 2, float xlocal = -3.0f) {
		float dylocal = std::get<1>(size)/float(count - 1);
		float dy = (std::get<1>(bb.max())-std::get<1>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<1>(bb.min())+i*dy);
			add(Text(xlocal, std::get<1>(size) - i*dylocal,stext.str())).class_({"label","ylabel"}).style().text_anchor(text_anchor_end).dominant_baseline(dominant_baseline_middle);
		}
		return (*this);
	}

};
	
};
