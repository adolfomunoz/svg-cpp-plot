#pragma once

#include <limits>
#include <string>
#include <type_traits>

namespace svg_cpp_plot {

template<typename T>
struct is_2d_point {
	static constexpr bool value = std::tuple_size_v<T> == 2 &&
			std::is_floating_point_v<std::tuple_element_t<0,T>> &&
			std::is_floating_point_v<std::tuple_element_t<1,T>>;
};

template<typename T>
constexpr bool is_2d_point_v = is_2d_point<T>::value;


class BoundingBox {
	std::tuple<float, float> min, max;
public:
	BoundingBox(float xmin, float ymin, float xmax, float ymax) :
		min(xmin,ymin), max(xmax,ymax) { }

	BoundingBox(const std::tuple<float,float>& min, const std::tuple<float, float>& max) :
		min(min), max(max) { }
		
	BoundingBox() : BoundingBox(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min()) {}
		
	BoundingBox& join(const BoundingBox& that) { 
		if (std::get<0>(that.min)<std::get<0>(min)) std::get<0>(min)=std::get<0>(that.min);
		if (std::get<1>(that.min)<std::get<1>(min)) std::get<1>(min)=std::get<1>(that.min);
		if (std::get<0>(that.max)>std::get<0>(max)) std::get<0>(max)=std::get<0>(that.max);
		if (std::get<1>(that.max)>std::get<1>(max)) std::get<1>(max)=std::get<1>(that.max);
		return (*this);
	}

	//A point
	BoundingBox& join(const std::tuple<float, float>& p) {
		return join(BoundingBox(p,p));
	}

	//Expand by a distance in x and y
	BoundingBox& expand(float dx, float dy) {
		std::get<0>(min)-=dx;
		std::get<1>(min)-=dy;
		std::get<0>(max)+=dx;
		std::get<1>(max)+=dy;
		return(*this);
	}
	//Expand by a distance
	BoundingBox& expand(float f) {
		return expand(f,f);
	}
	
	//Expand by a relative percentage
	BoundingBox& expand_rate(float rate) {
		return expand(rate*(std::get<0>(max)-std::get<0>(min)),
					rate*(std::get<1>(max)-std::get<1>(min)));
	}

	std::string to_string() const {
		std::stringstream sstr;
		sstr<<std::get<0>(min)<<" "<<std::get<1>(min)<<" "<<
				(std::get<0>(max) - std::get<0>(min))<<" "<<
				(std::get<1>(max) - std::get<1>(min));
		return sstr.str();
	}
};

};

