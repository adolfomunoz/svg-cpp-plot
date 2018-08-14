#pragma once 

#include "object.h"
#include "presentation-attributes.h"

namespace svg_cpp_plot {

class Poly : public Terminal, public PresentationAttributes<Poly> {
public:
	//PointList is any iterable thing of std::tuple<float,float>
	template<typename PointList> 
	Poly(const std::string& tag, const PointList& pl) : Terminal(tag) 
	{
		std::stringstream sstr;
		for (auto [x,y] : pl) 
			sstr<<x<<","<<y<<" ";
		set("points",sstr.str());
	}

	std::list<std::tuple<float,float>> points() const noexcept {
		std::stringstream sstr(get("points").value_or(""));
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
	Polygon(const PointList& pl) : Poly("polygon",pl) { }
	Polygon(std::initializer_list<std::tuple<float,float>> pl) : Poly("polygon",pl) { }
};

class Polyline : public Poly {
public:
	template<typename PointList>
	Polyline(const PointList& pl) : Poly("polyline",pl) { fill("none").stroke("black"); }
	Polyline(std::initializer_list<std::tuple<float,float>> pl) : Poly("polyline",pl) { fill("none").stroke("black"); }
};


}
