#pragma once

#include <type_traits>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "point-3d.h"

namespace svg_cpp_plot {
	
template<typename P>
std::tuple<float, float> point_3d_to_2d(const P& p) {
	static_assert(is_3d_point_v<P>,"Should be a 3D point");
	return std::tuple<float, float>(std::get<0>(p), std::get<1>(p));
}

using Matrix = std::array<std::array<float,4>,4>;

namespace matrix3 {
constexpr Matrix oblique   = { {{1,0,1.0/sqrt(2),0},{0,1,1.0/sqrt(2),0},{0, 0, 1, 0},{0,0,0,1}} };
constexpr Matrix isometric = { {{1.0/sqrt(2),0,-1.0/sqrt(2),0},
	                       {1.0/sqrt(6),2.0/sqrt(6),1.0/sqrt(6),0},
			       {1.0/sqrt(3), -1.0/sqrt(3), 1.0/sqrt(3), 0},
			       {0,0,0,1}} };
constexpr Matrix zxy = { {{0,0,1,0},{1,0,0,0},{0,1,0,0},{0,0,0,1}} };
constexpr Matrix zyx = { {{0,0,1,0},{0,1,0,0},{1,0,0,0},{0,0,0,1}} };

}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
	for (int f=0;f<4;++f) {
		os<<"[ ";
		for (int c=0;c<4;++c) os<<std::setw(10)<<m[f][c]<<" ";
		os<<"]"<<std::endl;
	}
	return os;
}
	
constexpr Matrix operator*(const Matrix& m1, const Matrix& m2) noexcept {
	Matrix r{ {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}} };
	for (int f = 0;f<4;++f) for (int c = 0;c<4;++c) for (int i =0; i<4; ++i) {
		r[f][c] += m1[f][i]*m2[i][c];
	}
	return r;
}

template<typename P>
constexpr std::tuple<float, float, float> transform_point(const Matrix& m, const P& p) noexcept {
	static_assert(is_3d_point_v<P>,"Can only transform 3d points");
	float w = std::get<0>(p)*m[3][0] + std::get<1>(p)*m[3][1] + std::get<2>(p)*m[3][2] + m[3][3];
	return std::tuple<float, float, float>(
		(std::get<0>(p)*m[0][0] + std::get<1>(p)*m[0][1] + std::get<2>(p)*m[0][2] + m[0][3])/w,
		(std::get<0>(p)*m[1][0] + std::get<1>(p)*m[1][1] + std::get<2>(p)*m[1][2] + m[1][3])/w,
		(std::get<0>(p)*m[2][0] + std::get<1>(p)*m[2][1] + std::get<2>(p)*m[2][2] + m[2][3])/w
		);
}  

template<typename P>
constexpr std::tuple<float, float, float> transform_direction(const Matrix& m, const P& p) noexcept {
	static_assert(is_3d_point_v<P>,"Can only transform 3d directions");
	return std::tuple<float, float, float>(
		(std::get<0>(p)*m[0][0] + std::get<1>(p)*m[0][1] + std::get<2>(p)*m[0][2]),
		(std::get<0>(p)*m[1][0] + std::get<1>(p)*m[1][1] + std::get<2>(p)*m[1][2]),
		(std::get<0>(p)*m[2][0] + std::get<1>(p)*m[2][1] + std::get<2>(p)*m[2][2])
		);
}    
};
