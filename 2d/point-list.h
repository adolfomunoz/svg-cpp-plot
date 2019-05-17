#pragma once

#include "../primitives/point.h"
#include <list>

namespace svg_cpp_plot {
namespace _2d {

template<typename T>
class PointList {
protected:
	std::list<std::tuple<float,float>> point_list;
public:
	template<typename PL>
	PointList(const PL& pl) : point_list(pl) { }
	template<typename PL>
	PointList(PL&& pl) : point_list(std::forward<PL>(pl)) { }
	
	PointList(std::initializer_list<std::tuple<float,float>>&& pl) :  point_list(std::forward<std::initializer_list<std::tuple<float,float>>>(pl)) { }

	PointList& add_point(float x, float y) {
		point_list.push_back(std::tuple<float,float>(x,y)); 
		return static_cast<T&>(*this);
	}

	template<typename P>
	PointList& add_point(const P& p) {
		static_assert(is_2d_point_v<P>, "Requires 2d points");
		return add_point(std::get<0>(p),std::get<1>(p));
		return static_cast<T&>(*this);
	}

	template<typename P>
	PointList& add_points(const std::initializer_list<P>& ps) {
		static_assert(is_2d_point_v<P>, "Requires 2d points");
		for (const P& p : ps) this->add_point(p);
		return static_cast<T&>(*this);
	}

};

}
}
