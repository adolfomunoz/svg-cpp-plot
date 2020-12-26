#pragma once

#include "../core/element-generator.h"
#include "transform.h"
#include "curve.h"
#include "../primitives/point.h"
#include "../primitives/line.h"
#include <cmath>
#include <type_traits>
#include "point-list.h"

namespace svg_cpp_plot {
namespace _2d {



class points : public PointList, public ElementGenerator<Matrix, svg_cpp_plot::Group>, public Attributes<points>, public GraphicalAttributes<points>, public StyleAttributes<points>, public PresentationAttributes<points> {
	std::shared_ptr<_2d::Element> symbol_;
public:
	template<typename T>
	points& set_symbol(const T& t,
		std::enable_if_t<std::is_base_of_v<_2d::Element,T>,int> dummy = 0) {
		
		symbol_ = std::make_shared<T>(t);
		return (*this);
	}
	
	points& set_symbol(const std::shared_ptr<_2d::Element>& t) {
		symbol_ = t;
		return (*this);
	}
	
	_2d::Element& symbol() { return *symbol_; }
	const _2d::Element& symbol() const { return *symbol_; }
	
private:
	void set_default_symbol() {
		set_symbol(circle({0,0},1).stroke_width(0).fill(black));
	}

public:
	
	points() { set_default_symbol(); }
	
	points(std::list<std::tuple<float,float>>&& pl) :
	   PointList(std::forward<std::list<std::tuple<float,float>>>(pl)) {
		set_default_symbol(); 
	}
	
	points(const std::list<std::tuple<float,float>>& pl) :
	   PointList(pl) {
		set_default_symbol(); 
	}
	
	points(std::initializer_list<std::tuple<float,float>>&& pl) : 	PointList(std::forward<std::initializer_list<std::tuple<float,float>>>(pl)) {
		set_default_symbol(); 
	}
	
	
	svg_cpp_plot::Group element(const Matrix& m) const noexcept override {
		svg_cpp_plot::Group g;
		for (auto p : point_list()) {
			auto pt = transform_point(m,p);
			g.add(
				FixedGenerator(translate(pt),symbol_));
		}
		return g;
	}
};


auto point(const std::tuple<float,float>& p) {
	return points({p}).class_("point");
}



}
}
