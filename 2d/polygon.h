#pragma once

#include "../core/element-generator.h"
#include "transform.h"
#include "../primitives/point.h"
#include "../primitives/poly.h"
#include <cmath>
#include <type_traits>
#include "point-list.h"

namespace svg_cpp_plot {
namespace _2d {

class polygon : public PointList, public ElementGenerator<Matrix, Polygon>, public Attributes<polygon>, public GraphicalAttributes<polygon>, public StyleAttributes<polygon>, public PresentationAttributes<polygon> {
public:
	using PointList::PointList;
	Polygon element(const Matrix& m) const noexcept override {
		Polygon g;
		for (auto p : point_list()) g.add_point(transform_point(m,p)); 
		return g;
	}
};

auto rect(const std::tuple<float,float>& p1, const std::tuple<float,float>& p2) {
	return polygon({p1,std::tuple(std::get<0>(p1),std::get<1>(p2)),p2,std::tuple(std::get<0>(p2),std::get<1>(p1))}).class_("rectangle");
}

auto triangle(const std::tuple<float,float>& p1, 
			  const std::tuple<float,float>& p2, 
			  const std::tuple<float,float>& p3) {
	return polygon({p1,p2,p3}).class_("triangle");
}

}
}
