#pragma once

#include <cmath>
#include <tuple>
#include <array>
#include <type_traits>

namespace svg_cpp_plot {

template <typename T>
struct is_2d_point {
	static constexpr bool value = false;
};

template<typename E1, typename E2>
struct is_2d_point<std::tuple<E1, E2>> {
	static constexpr bool value = 
		std::is_floating_point_v<E1> &&
		std::is_floating_point_v<E2>;
};

template<typename E>
struct is_2d_point<std::array<E,2>> {
	static constexpr bool value = std::is_floating_point_v<E>;
};


template<typename T>
constexpr bool is_2d_point_v = is_2d_point<T>::value;


template<typename P0, typename P1, typename = std::enable_if_t<is_2d_point_v<P0> && is_2d_point_v<P1>> >
constexpr std::tuple<float, float> operator+(const P0& p0, const P1& p1) {
	return std::tuple<float, float>(std::get<0>(p0)+std::get<0>(p1), std::get<1>(p0) + std::get<1>(p1));
}

template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
constexpr std::tuple<float, float> operator*(const P& p0, float f) {
	return std::tuple<float, float>(std::get<0>(p0)*f, std::get<1>(p0)*f);
}

template<typename P0, typename P1, typename = std::enable_if_t<is_2d_point_v<P0> && is_2d_point_v<P1>> >
constexpr std::tuple<float, float> operator-(const P0& p0, const P1& p1) {
	return p0 + p1*(-1.0f);
}

template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
constexpr std::tuple<float, float> operator*(float f, const P& p0) {
	return p0*f;
}


template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
constexpr std::tuple<float, float> operator/(const P& p0, float f) {
	return p0*(1.0f/f);
}

}
