#pragma once

#include <type_traits>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <array>
#include <functional>
#include "../primitives/point.h"
#include "../primitives/group.h"
#include "../primitives/clip-path.h"
#include "../primitives/defs.h"
#include "../primitives/mask.h"

namespace svg_cpp_plot {
namespace _2d {

using Matrix = std::array<std::array<float,3>,3>;
constexpr Matrix identity = Matrix{ {{1,0,0},{0,1,0}, {0,0,1}} };
constexpr Matrix scale(float x, float y) {
	return Matrix{ {{x,0,0},{0,y,0},{0,0,1}} };
}
constexpr Matrix scale(const std::tuple<float,float>& s) {
	return scale(std::get<0>(s),std::get<1>(s));
}
constexpr Matrix translate(float x, float y) {
	return Matrix{ {{1,0,x},{0,1,y},{0,0,1}} };
}
constexpr Matrix translate(const std::tuple<float,float>& t) {
	return translate(std::get<0>(t),std::get<1>(t));
}	
constexpr Matrix rotate(float a) {
	return Matrix{ {{std::cos(a),std::sin(a),0},{-std::sin(a),std::cos(a),0},{0,0,1}} };
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
	for (int f=0;f<3;++f) {
		os<<"[ ";
		for (int c=0;c<3;++c) os<<std::setw(10)<<m[f][c]<<" ";
		os<<"]"<<std::endl;
	}
	return os;
}

struct MatrixProduct {
constexpr svg_cpp_plot::_2d::Matrix operator()(const svg_cpp_plot::_2d::Matrix& m1, const svg_cpp_plot::_2d::Matrix& m2) const noexcept {
	svg_cpp_plot::_2d::Matrix r{ {{0,0,0},{0,0,0},{0,0,0}} };
	for (int f=0;f<3;++f) for (int c=0;c<3;++c) for (int i=0;i<3;++i) r[f][c] += m1[f][i]*m2[i][c];
	return r;
}
};

constexpr svg_cpp_plot::_2d::Matrix operator*(const svg_cpp_plot::_2d::Matrix& m1, const svg_cpp_plot::_2d::Matrix& m2) noexcept {
	svg_cpp_plot::_2d::Matrix r{ {{0,0,0},{0,0,0},{0,0,0}} };
	for (int f=0;f<3;++f) for (int c=0;c<3;++c) for (int i=0;i<3;++i) r[f][c] += m1[f][i]*m2[i][c];
	return r;
}

template<typename P>
constexpr std::tuple<float, float> transform_point(const Matrix& m, const P& p) noexcept {
	static_assert(is_2d_point_v<P>,"Can only transform 2d points");
	float w = std::get<0>(p)*m[2][0] + std::get<1>(p)*m[2][1] + m[2][2];
	return std::tuple<float, float>(
		(std::get<0>(p)*m[0][0] + std::get<1>(p)*m[0][1] + m[0][2])/w,
		(std::get<0>(p)*m[1][0] + std::get<1>(p)*m[1][1] + m[1][2])/w);
}  

template<typename P>
constexpr std::tuple<float, float> transform_direction(const Matrix& m, const P& p) noexcept {
	static_assert(is_2d_point_v<P>,"Can only transform 2d directions");
	return std::tuple<float, float>(
		(std::get<0>(p)*m[0][0] + std::get<1>(p)*m[0][1]),
		(std::get<0>(p)*m[1][0] + std::get<1>(p)*m[1][1]));
}

class Group : public GroupGenerator<Matrix,MatrixProduct> {
public:
    Group(const Matrix& m = identity) : GroupGenerator<Matrix,MatrixProduct>(m, MatrixProduct()) {}
};

inline auto group(const Matrix& m = identity) {
	return Group(m);
}

inline auto clip_path(const svg_cpp_plot::_2d::Matrix& m = identity) {
	return ClipPathGenerator(m, MatrixProduct());
}
inline auto defs(const svg_cpp_plot::_2d::Matrix& m = identity) {
	return DefsGenerator(m, MatrixProduct());
}
inline auto mask(const svg_cpp_plot::_2d::Matrix& m = identity) {
	return MaskGenerator(m, MatrixProduct());
}


using Element = Generator<svg_cpp_plot::_2d::Matrix>;

}
}

using svg_cpp_plot::_2d::operator*;

