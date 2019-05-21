#pragma once

#include "../primitives/group.h"
#include "../primitives/line.h"

namespace svg_cpp_plot {

class Points : public Group {
public:
	Points& add_point(float x, float y) {
		this->add(Line(x,y,x,y)); return (*this);
	}

	template<typename P>
	Points& add_point(const P& p) {
		static_assert(is_2d_point_v<P>, "Requires 2d points");
		return add(std::get<0>(p),std::get<1>(p));
	}

	template<typename P>
	Points& add_points(const std::initializer_list<P>& ps) {
		static_assert(is_2d_point_v<P>, "Requires 2d points");
		for (const P& p : ps) this->add_point(p);
		return (*this);
	}

};

}
