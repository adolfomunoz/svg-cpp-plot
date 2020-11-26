#pragma once

#include "../core/element-generator.h"
#include "transform.h"
#include "../primitives/point.h"
#include "../primitives/text.h"
#include <cmath>
#include <type_traits>

namespace svg_cpp_plot {
namespace _2d {

template<typename P>
class primitive : public ElementGenerator<Matrix, P>, public Attributes<primitive<P>> {
	P p;
public:
	primitive(const P& p) : p(p) {}
	P element(const Matrix& m) const noexcept override {
		P prim = p;
		if constexpr(std::is_base_of<GraphicalAttributes<P>,P>::value)
			prim.transform({matrix(m[0][0],m[0][1],m[1][0],m[1][1],m[0][2],m[1][2])});
		return prim;
	}
	
	P& get() { return p; }
	const P& get() const { return p; }
	
};

}
}
