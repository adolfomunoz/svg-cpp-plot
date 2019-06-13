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

class polyline : public PointList<polyline>, public ElementGenerator<Matrix, Polyline>, public Attributes<polyline>, public GraphicalAttributes<polyline>, public StyleAttributes<polyline>, public PresentationAttributes<polyline> {
public:
	using PointList::PointList;
	Polyline element(const Matrix& m) const noexcept override {
		Polyline g;
		for (auto p : point_list) g.add_point(transform_point(m,p));
		return g;
	}
};

auto line(const std::tuple<float,float>& p1, const std::tuple<float,float>& p2) {
	return polyline({p1,p2}).class_("line");
}

}
}
