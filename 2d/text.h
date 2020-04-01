#pragma once

#include "../core/element-generator.h"
#include "transform.h"
#include "../primitives/point.h"
#include "../primitives/text.h"
#include <cmath>
#include <type_traits>

namespace svg_cpp_plot {
namespace _2d {

class text : public ElementGenerator<Matrix, Text>, public Attributes<text>, public GraphicalAttributes<text>, public StyleAttributes<text>, public PresentationAttributes<text>, public TextPresentationAttributes<text> {
	std::tuple<float, float> position;
	std::string str;
public:
	text(const std::tuple<float, float>& position, std::string_view str) : position(position), str(str) {}
	Text element(const Matrix& m) const noexcept override {
		Text t(std::get<0>(position), std::get<1>(position), str);
		t.transform({matrix(m[0][0],m[0][1],m[1][0] ,m[1][1],m[0][2],m[1][2])});
		return t;
	}
};

}
}
