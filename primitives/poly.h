#pragma once 

#include "../core/element.h"
#include "point.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/style-attributes.h"
#include "../attributes/graphical-attributes.h"

namespace svg_cpp_plot {

class Poly : public Terminal, public Attributes<Poly>, public GraphicalAttributes<Poly>, public StyleAttributes<Poly>, public PresentationAttributes<Poly> {
public:
	//PointList is any iterable thing of std::tuple<float,float> (or point, generally speaking)
	template<typename PointList> 
	Poly(const std::string& tag, const PointList& pl) : Terminal(tag) 
	{
		std::stringstream sstr;
		for (auto [x,y] : pl) 
			sstr<<x<<","<<y<<" ";
		set("points",sstr.str());
	}

	Poly& add_point(float x, float y) {
		std::stringstream sstr;
		sstr << get_default("points",std::string(""))<<x<<","<<y<<" ";
		set("points", sstr.str());
		return (*this);
	}

	template<typename P>
	Poly& add_point(const P& p) {
		static_assert(is_2d_point_v<P>,"Expecting a 2D point");
		return add_point(std::get<0>(p), std::get<1>(p));
	}

	std::list<std::tuple<float,float>> points() const noexcept {
		std::stringstream sstr(get_default("points",std::string("")));
		std::list<std::tuple<float,float>> l;
		std::tuple<float,float> t;
		char dummy;
		sstr>>std::get<0>(t)>>dummy>>std::get<1>(t);//>>dummy;
		while (sstr) {
			l.push_back(t);
			sstr>>std::get<0>(t)>>dummy>>std::get<1>(t);//>>dummy;
		}
		return l;
	};
	
	BoundingBox bounding_box() const noexcept override { 
		BoundingBox bb;
		for (auto p : points()) bb.join(p);
		bb.expand(get_float("stroke-width",0.0f));
		return bb;
	}
};

class Polygon : public Poly {
public:
	template<typename PointList>
	Polygon(const PointList& pl = {}) : Poly("polygon",pl) { }
	Polygon(std::initializer_list<std::tuple<float,float>> pl) : Poly("polygon",pl) { }
	Polygon() : Polygon(std::list<std::tuple<float,float>>()) { }
};

class Polyline : public Poly {
public:
	template<typename PointList>
	Polyline(const PointList& pl) : Poly("polyline",pl) { fill(none); }
	Polyline(std::initializer_list<std::tuple<float,float>> pl) : Poly("polyline",pl) { fill(none).stroke(black); }
	Polyline() : Polyline(std::list<std::tuple<float,float>>()) { }
};


}
