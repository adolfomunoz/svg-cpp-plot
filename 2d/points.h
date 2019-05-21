#pragma once

#include "../core/element-generator.h"
#include "transform.h"
#include "../primitives/point.h"
#include "../primitives/line.h"
#include <cmath>
#include <type_traits>
#include "point-list.h"

namespace svg_cpp_plot {
namespace _2d {

class points : public PointList<points>, public ElementGenerator<Matrix, svg_cpp_plot::Group>, public Attributes<points>, public GraphicalAttributes<points>, public StyleAttributes<points>, public PresentationAttributes<points> {
public:
	using PointList::PointList;
	svg_cpp_plot::Group element(const Matrix& m) const noexcept override {
		svg_cpp_plot::Group g;
		for (auto p : point_list) {
			auto pt = transform_point(m,p);
			g.add(Line(std::get<0>(pt),std::get<1>(pt),std::get<0>(pt),std::get<1>(pt))).stroke_linecap(stroke_linecap_round);
		}
		return g;
	}
};

}
}
