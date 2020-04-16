#pragma once

#include "../primitives/point.h"
#include <list>

namespace svg_cpp_plot {
namespace _2d {

class PointList {
protected:
	std::list<std::tuple<float,float>> point_list_;
public:
	PointList() {}
	template<typename PL>
	PointList(const PL& pl) : point_list_(pl) { }
	template<typename PL>
	PointList(PL&& pl) : point_list_(std::forward<PL>(pl)) { }

	PointList(const PointList& pl) : point_list_(pl.point_list_) { }
	PointList(PointList&& pl) : point_list_(std::move(pl.point_list_)) { }
	
	
	PointList(std::initializer_list<std::tuple<float,float>>&& pl) :  point_list_(std::forward<std::initializer_list<std::tuple<float,float>>>(pl)) { }

	PointList& add_point(float x, float y) {
		point_list_.push_back(std::tuple<float,float>(x,y)); 
		return *this;
	}

	template<typename P>
	PointList& add_point(const P& p) {
		static_assert(is_2d_point_v<P>, "Requires 2d points");
		return add_point(std::get<0>(p),std::get<1>(p));
	}

	template<typename P>
	PointList& add_points(const std::initializer_list<P>& ps) {
		static_assert(is_2d_point_v<P>, "Requires 2d points");
		for (const P& p : ps) this->add_point(p);
		return *this;
	}
	
	PointList& add_points(const std::initializer_list<std::tuple<float,float>>& ps) {
		for (const auto& p : ps) this->add_point(p);
		return *this;
	}

    const std::list<std::tuple<float,float>>& point_list() const { return point_list_; }
};

}
}
