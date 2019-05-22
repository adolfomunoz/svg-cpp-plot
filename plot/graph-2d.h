#pragma once

#include "../2d/transform.h"
#include "../2d/polygon.h"
#include "../2d/polyline.h"
#include "../2d/text.h"
#include "../attributes/style.h"
#include <cassert>

namespace svg_cpp_plot {
		
using _2d::operator*;
class Graph2D : public _2d::Group {
	std::tuple<float, float> size; //<-- The group's bounding box should be (0,0) to size
	BoundingBox bb; //<-- This is the bounding box in world's coordinates, that is fit into (0,0) size
	_2d::Group& area_;

	auto xtick(float xlocal, float height, float ylocal) {
		return _2d::line({xlocal,ylocal-0.5f*height},{xlocal,ylocal+0.5f*height}).class_("xtick");
	}

	auto ytick(float ylocal, float width, float xlocal) {
		return _2d::line({xlocal-0.5f*width,ylocal},{xlocal+0.5f*width,ylocal}).class_("ytick");
	}



public:
	auto& area() noexcept { return area_; }
	auto& border() noexcept { 
		return this->add(_2d::rect({0,0},size)).fill(none).pointer_events(pointer_events_none).class_("border");
	}

	Graph2D(const std::tuple<float, float>& size, const BoundingBox& bb):
	    _2d::Group(_2d::group(_2d::identity)), size(size),bb(bb),
		area_(this->add(_2d::group(_2d::scale(std::get<0>(size)/bb.width(),-std::get<1>(size)/bb.height())*
		    _2d::translate(-std::get<0>(bb.min()),-std::get<1>(bb.max())))))
	{
		area().fill(none).vector_effect(non_scaling_stroke).class_("area").style().pointer_events(pointer_events_all);
		this->add(_2d::clip_path()).id(this->id()+"clipper").add(_2d::rect({0,0},size));
		area().set("clip-path",std::string("url(#")+this->id()+"clipper)");
	}


	auto& axis(float x = 0.0f, float y = 0.0f) {
		auto& g = area().add(_2d::group()).class_("axis");
		g.add(_2d::line({x,-2.e4},{x,2.e4}));
		g.add(_2d::line({-2.e4,y},{2.e4,y}));
		return g;
	}

	auto& xticks(int count = 2, float height = 3.0f, float ylocal = 0.0f) {
		auto& g = add(_2d::group()).class_("xticks");		
		float dx = std::get<0>(size)/float(count - 1);
		for (int i = 0;i<count;++i) g.add(xtick(i*dx, height, std::get<1>(size) - ylocal));
		return g;
	}

	auto& yticks(int count = 2, float width = 3.0f, float xlocal = 0.0f) {
		auto& g = add(_2d::group()).class_("yticks");		
		float dy = std::get<1>(size)/float(count - 1);
		for (int i = 0;i<count;++i) g.add(ytick(i*dy, width, xlocal));
		return g;
	}

/*
	void ticks(int xs = 2, int ys = 2, float size = 3.0f, float xlocal = 0.0f, float ylocal = 0.0f) {
		xticks(xs,size,ylocal);
		yticks(ys,size,xlocal);
	}
*/
 
	auto& xlabels(int count = 2, float ylocal = -3.0f) {
		auto& g = add(_2d::group()).class_("labels");
		float dxlocal = std::get<0>(size)/float(count - 1);
		float dx = (std::get<0>(bb.max())-std::get<0>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<0>(bb.min())+i*dx);
			g.add(_2d::text({i*dxlocal,std::get<1>(size) - ylocal},stext.str())).font_size(std::get<0>(size)/20).class_({"label","xlabel"}).style().text_anchor(text_anchor_middle).dominant_baseline(dominant_baseline_hanging);
		}
		return g;
	}

	auto& ylabels(int count = 2, float xlocal = -3.0f) {
		auto& g = add(_2d::group()).class_("labels");
		float dylocal = std::get<1>(size)/float(count - 1);
		float dy = (std::get<1>(bb.max())-std::get<1>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<1>(bb.min())+i*dy);
			g.add(_2d::text({xlocal, std::get<1>(size) - i*dylocal},stext.str())).font_size(std::get<0>(size)/20).class_({"label","ylabel"}).style().text_anchor(text_anchor_end).dominant_baseline(dominant_baseline_middle);
		}
		return g;
	}
};
	
};
