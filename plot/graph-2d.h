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

	auto xtick(float xlocal, float height, float ylocal, const std::string& classname) {
		return this->add(Line(xlocal,ylocal-0.5f*height,xlocal,ylocal+0.5f*height)).class_(classname);
	}

	auto ytick(float ylocal, float width, float xlocal, const std::string& classname) {
		return this->add(Line(xlocal-0.5f*width,ylocal,xlocal+0.5f*width,ylocal)).class_(classname);
	}



public:
	Style& style() noexcept { return style_; }
	Group& area() noexcept { return area_; }
	Rect& border() noexcept { return border_; }
	//Note that if it already exists it is not added but just returned.
//	StyleEntry& all_plots() noexcept { return style().add_class("plot"); }

	Graph2D(const std::tuple<float, float>& size, const BoundingBox& bb):
	       size(size),bb(bb),style_(Group::add(Style())),area_(Group::add(Group())),border_(Group::add(Rect({0,0},size)))
	{
		//The matrix thing does not seem to be working, we go with scale and traslation
		area_.id("area");
//		area_.transform(matrix(std::get<0>(size)/bb.width(),0,-std::get<0>(bb.min()),0,std::get<1>(size)/bb.height(), -std::get<1>(bb.min()))).vector_effect(non_scaling_stroke);
		area_.transform({scale(std::get<0>(size)/bb.width(),std::get<1>(size)/bb.height()),translate(-std::get<0>(bb.min()),-std::get<1>(bb.min()))});
		area_.clip_path().add(border()).transform({translate(std::get<0>(bb.min()),std::get<1>(bb.min())),scale(bb.width()/std::get<0>(size),bb.height()/std::get<1>(size))});
		border().fill(none);
		style().add_id("area").nest("*").vector_effect(non_scaling_stroke).nest("*").vector_effect(non_scaling_stroke);
		style().add("text").font_size(float(std::get<1>(size))/float(20));
		style().add("path").stroke_linecap(stroke_linecap_round).fill_opacity(0.25);
	}

	Group& axis(float x = 0.0f, float y = 0.0f) {
		Group g;
		g.add(Line(x,-2.e10,x,2.e10)).vector_effect(non_scaling_stroke);
		g.add(Line(-2.e10,y,2.e10,y)).vector_effect(non_scaling_stroke);
		return area().add(g);
	}

	StyleEntry& xticks(int count = 2, float height = 3.0f, float ylocal = 0.0f, std::string classname = "xticks") {
		float dx = std::get<0>(size)/float(count - 1);
		for (int i = 0;i<count;++i) xtick(i*dx, height, std::get<1>(size) - ylocal, classname);
		return style().add_class(classname).stroke_linecap(stroke_linecap_round).fill(none);
	}

	StyleEntry& yticks(int count = 2, float width = 3.0f, float xlocal = 0.0f, std::string classname = "yticks") {
		float dy = std::get<1>(size)/float(count - 1);
		for (int i = 0;i<count;++i) ytick(i*dy, width, xlocal, classname);
		return style().add_class(classname).stroke_linecap(stroke_linecap_round).fill(none);
	}

	StyleEntry& ticks(int xs = 2, int ys = 2, float size = 3.0f, float xlocal = 0.0f, float ylocal = 0.0f, std::string classname = "ticks") {
		xticks(xs,size,ylocal, classname);
		return yticks(ys,size,xlocal,classname);
	}

	StyleEntry& xlabels(int count = 2, float ylocal = -3.0f, std::string classname = "xlabels") {
		float dxlocal = std::get<0>(size)/float(count - 1);
		float dx = (std::get<0>(bb.max())-std::get<0>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<0>(bb.min())+i*dx);
			add(Text(i*dxlocal,std::get<1>(size) - ylocal,stext.str())).class_(classname);
		}
		return style().add_class(classname).text_anchor(text_anchor_middle).dominant_baseline(dominant_baseline_hanging);
	}

	StyleEntry& ylabels(int count = 2, float xlocal = -3.0f, std::string classname = "ylabels") {
		float dylocal = std::get<1>(size)/float(count - 1);
		float dy = (std::get<1>(bb.max())-std::get<1>(bb.min()))/float(count - 1);
		for (int i = 0;i<count;++i) {
			std::stringstream stext;	stext<<(std::get<1>(bb.min())+i*dy);
			add(Text(xlocal, std::get<1>(size) - i*dylocal,stext.str())).class_(classname);
		}
		return style().add_class(classname).text_anchor(text_anchor_end).dominant_baseline(dominant_baseline_middle);
	}

};
	
};
