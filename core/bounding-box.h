#pragma once

#include <limits>
#include <string>
#include <type_traits>
#include "object.h"

namespace svg_cpp_plot {


class BoundingBox : public Object {
	std::tuple<float, float> min_, max_;
public:
	BoundingBox(float xmin, float ymin, float xmax, float ymax) :
		min_(xmin,ymin), max_(xmax,ymax) { }

	BoundingBox(const std::tuple<float,float>& min, const std::tuple<float, float>& max) :
		min_(min), max_(max) { }
		
	BoundingBox() : BoundingBox(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min()) {}

	const std::tuple<float, float>& min() const noexcept { return min_; }
	const std::tuple<float, float>& max() const noexcept { return max_; }
		
	BoundingBox& join(const BoundingBox& that) noexcept { 
		if (std::get<0>(that.min())<std::get<0>(min())) std::get<0>(min_)=std::get<0>(that.min());
		if (std::get<1>(that.min())<std::get<1>(min())) std::get<1>(min_)=std::get<1>(that.min());
		if (std::get<0>(that.max())>std::get<0>(max())) std::get<0>(max_)=std::get<0>(that.max());
		if (std::get<1>(that.max())>std::get<1>(max())) std::get<1>(max_)=std::get<1>(that.max());
		return (*this);
	}

	//A point
	BoundingBox& join(const std::tuple<float, float>& p) noexcept {
		return join(BoundingBox(p,p));
	}

	float width() const noexcept { return std::get<0>(max())-std::get<0>(min()); }
	float height() const noexcept { return std::get<1>(max())-std::get<1>(min()); }

	//Expand by a distance in x and y
	BoundingBox& expand(float dx, float dy) noexcept {
		std::get<0>(min_)-=dx;
		std::get<1>(min_)-=dy;
		std::get<0>(max_)+=dx;
		std::get<1>(max_)+=dy;
		return(*this);
	}
	//Expand by a distance
	BoundingBox& expand(float f) noexcept {
		return expand(f,f);
	}
	
	//Expand by a relative percentage
	BoundingBox& expand_rate(float rate) noexcept {
		return expand(rate*(std::get<0>(max())-std::get<0>(min())),
					rate*(std::get<1>(max())-std::get<1>(min())));
	}

	bool is_inside(const std::tuple<float,float>& p) noexcept {
		return (std::get<0>(p) >= std::get<0>(min())) &&
		       (std::get<1>(p) >= std::get<1>(min())) &&
		       (std::get<0>(p) <= std::get<0>(max())) &&
		       (std::get<1>(p) <= std::get<1>(max()));
	}

	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<std::get<0>(min())<<" "<<std::get<1>(min())<<" "<<
				(std::get<0>(max()) - std::get<0>(min()))<<" "<<
				(std::get<1>(max()) - std::get<1>(min()));
		return sstr.str();
	}
};

};

