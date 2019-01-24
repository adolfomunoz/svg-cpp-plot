#pragma once

#include <cmath>
#include <type_traits>

namespace svg_cpp_plot {

template <typename T>
struct is_3d_point {
	static constexpr bool value = false;
};

template<typename E1, typename E2, typename E3>
struct is_3d_point<std::tuple<E1, E2, E3>> {
	static constexpr bool value = 
		std::is_floating_point_v<E1> &&
		std::is_floating_point_v<E2> &&
		std::is_floating_point_v<E3>;
};

template<typename E>
struct is_3d_point<std::array<E,3>> {
	static constexpr bool value = std::is_floating_point_v<E>;
};


template<typename T>
constexpr bool is_3d_point_v = is_3d_point<T>::value;



template<typename P0, typename P1, typename = std::enable_if_t<is_3d_point_v<P0> && is_3d_point_v<P1>> >
constexpr std::tuple<float, float, float> operator+(const P0& p0, const P1& p1) {
	return std::tuple<float, float, float>(
			std::get<0>(p0)+std::get<0>(p1), std::get<1>(p0) + std::get<1>(p1), std::get<2>(p0) + std::get<2>(p1));
}

template<typename P, typename = std::enable_if_t<is_3d_point_v<P>> >
constexpr std::tuple<float, float, float> operator*(const P& p0, float f) {
	return std::tuple<float, float, float>(
			std::get<0>(p0)*f, std::get<1>(p0)*f, std::get<2>(p0)*f);
}

template<typename P0, typename P1, typename = std::enable_if_t<is_3d_point_v<P0> && is_3d_point_v<P1>> >
constexpr std::tuple<float, float, float> operator-(const P0& p0, const P1& p1) {
	return p0 + p1*(-1.0f);
}

template<typename P, typename = std::enable_if_t<is_3d_point_v<P>> >
constexpr std::tuple<float, float, float> operator*(float f, const P& p0) {
	return p0*f;
}


template<typename P, typename = std::enable_if_t<is_3d_point_v<P>> >
constexpr std::tuple<float, float, float> operator/(const P& p0, float f) {
	return p0*(1.0f/f);
}

/*
template<typename P0, typename P1, typename = std::enable_if_t<is_3d_point_v<P0> && is_3d_point_v<P1>> >
constexpr float dot(const P0& p0, const P1& p1) {
	return std::get<0>(p0)*std::get<0>(p1) + std::get<1>(p0)*std::get<1>(p1) + std::get<2>(p0)*std::get<2>(p1);
}
*/

template<typename P, typename = std::enable_if_t<is_3d_point_v<P>> >
constexpr float dot(const std::tuple<float, float, float>& p0, const P& p1) {
	return std::get<0>(p0)*std::get<0>(p1) + std::get<1>(p0)*std::get<1>(p1) + std::get<2>(p0)*std::get<2>(p1);
}

template<typename P, typename = std::enable_if_t<is_3d_point_v<P>> >
constexpr float dot(const P& p0, const std::tuple<float, float, float>& p1) {
	return std::get<0>(p0)*std::get<0>(p1) + std::get<1>(p0)*std::get<1>(p1) + std::get<2>(p0)*std::get<2>(p1);
}

constexpr float dot(const std::tuple<float, float, float>& p0, const std::tuple<float, float, float>& p1) {
	return std::get<0>(p0)*std::get<0>(p1) + std::get<1>(p0)*std::get<1>(p1) + std::get<2>(p0)*std::get<2>(p1);
}

template<typename P0, typename P1, typename = std::enable_if_t<is_3d_point_v<P0> && is_3d_point_v<P1>> >
constexpr std::tuple<float,float,float> cross(const P0& p0, const P1& p1) {
	return std::tuple<float, float, float>(std::get<1>(p0)*std::get<2>(p1) - std::get<2>(p0)*std::get<1>(p1),
					       std::get<2>(p0)*std::get<0>(p1) - std::get<0>(p0)*std::get<2>(p1),	
					       std::get<0>(p0)*std::get<1>(p1) - std::get<1>(p0)*std::get<0>(p1));	
}

template<typename P, typename = std::enable_if_t<is_3d_point_v<P>> >
constexpr std::tuple<float,float,float> cross(const std::tuple<float,float,float>& p0, const P& p1) {
	return std::tuple<float, float, float>(std::get<1>(p0)*std::get<2>(p1) - std::get<2>(p0)*std::get<1>(p1),
					       std::get<2>(p0)*std::get<0>(p1) - std::get<0>(p0)*std::get<2>(p1),	
					       std::get<0>(p0)*std::get<1>(p1) - std::get<1>(p0)*std::get<0>(p1));	
}



template<typename P, typename = std::enable_if_t<is_3d_point_v<P>> >
constexpr float length(const P& p) {
	return std::sqrt(dot(p,p));
}

template<typename P, typename = std::enable_if_t<is_3d_point_v<P>> >
constexpr std::tuple<float, float, float> normalize(const P& p) {
	return p/length(p);
}

template<typename P, typename OS, typename = std::enable_if_t<is_3d_point_v<P>> >
OS& operator<<(OS& os, const P& p) {
	os<<"[ "<<std::get<0>(p)<<" , "<<std::get<1>(p)<<" , "<<std::get<2>(p)<<" ]";
	return os;
}





}
