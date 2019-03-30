#pragma once

#include "../attributes/style.h"
#include <tuple>

namespace svg_cpp_plot {

class GraphStyle {
public:
	virtual Style style(const std::tuple<float, float>& size) const = 0;
};

namespace graph_style {

class Default : public GraphStyle {
public:
	Style style(const std::tuple<float, float>& size) const override {
		Style s;
		auto& plot = s.add_class("plot").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(40)).fill(none);
		plot.nth_of_type(1,7).stroke(red);
		plot.nth_of_type(2,7).stroke(yellow);
		plot.nth_of_type(3,7).stroke(pink);
		plot.nth_of_type(4,7).stroke(green);
		plot.nth_of_type(5,7).stroke(purple);
		plot.nth_of_type(6,7).stroke(orange);
		plot.nth_of_type(7,7).stroke(blue);

		s.add_class("plotarea").stroke_width(0).fill(none);

		s.add_class("axis").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(40)).stroke(black);
		s.add_class("border").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(40)).stroke(black);
		s.add_class("tick").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(80)).stroke(black);
		s.add_class("label").font_size(float(std::get<1>(size))/float(20));

		return s;
	}
};

class GrayScale : public GraphStyle {
public:
	Style style(const std::tuple<float, float>& size) const override {
		Style s;
		float w = float(std::get<1>(size))/float(40);
		auto& plot = s.add_class("plot").stroke_linecap(stroke_linecap_round).stroke_width(w).fill(none);
		plot.nth_of_type(1,7).stroke(black);
		plot.nth_of_type(2,7).stroke(grey);
		plot.nth_of_type(3,7).stroke(black).stroke_dasharray({2*w,2*w});
		plot.nth_of_type(4,7).stroke(black).stroke_dasharray({4*w,2*w,2*w,4*w});
		plot.nth_of_type(3,7).stroke(black).stroke_dasharray({4*w,4*w});
		plot.nth_of_type(4,7).stroke(black).stroke_dasharray({8*w,4*w,4*w,8*w});
		plot.nth_of_type(3,7).stroke(grey).stroke_dasharray({2*w,2*w});

		s.add_class("plotarea").stroke_width(0).fill(none);

		s.add_class("axis").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(40)).stroke(black);
		s.add_class("border").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(40)).stroke(black);
		s.add_class("tick").stroke_linecap(stroke_linecap_round).stroke_width(float(std::get<1>(size))/float(80)).stroke(black);
		s.add_class("label").font_size(float(std::get<1>(size))/float(20));

		return s;
	}
};


}
}
