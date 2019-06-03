#pragma once

#include "../core/element-generator.h"
#include "transform.h"
#include "../primitives/point.h"
#include "../primitives/path.h"
#include <cmath>
#include <type_traits>

namespace svg_cpp_plot {
namespace _2d {

constexpr std::tuple<float, float> operator*(const std::tuple<float,float>& p0, float f) {
	return std::tuple<float, float>(std::get<0>(p0)*f, std::get<1>(p0)*f);
}

std::ostream& operator<<(std::ostream& os, const std::tuple<float,float>& p) {
	os<<"("<<std::get<0>(p)<<","<<std::get<1>(p)<<")";
	return os;
}


template<typename F, typename DF>
class curve_derivative : public ElementGenerator<Matrix, Path>, public Attributes<curve_derivative<F,DF>>, public GraphicalAttributes<curve_derivative<F,DF>>, public StyleAttributes<curve_derivative<F,DF>>, public PresentationAttributes<curve_derivative<F,DF>> {
	F f;
	DF df;
	float tmin, tmax;
	unsigned int nsamples;
public:
	curve_derivative(F&& f, DF&& df, float tmin, float tmax, unsigned int nsamples = 100) :
		f(std::forward<F>(f)), df(std::forward<DF>(df)), tmin(tmin), tmax(tmax), nsamples(nsamples) {
			this->stroke_linecap(stroke_linecap_round).stroke_width(1).fill(none);
			static_assert(is_2d_point_v<decltype(f(tmin))>, "Function f should return a two dimensional point");
			static_assert(is_2d_point_v<decltype(df(tmin))>, "Derivative df should return a two dimensional point");
	}
	Path element(const Matrix& m) const noexcept override {
		float dt = (tmax - tmin)/float(nsamples-1);
		float t;
		Path path(transform_point(m,f(tmin)));
		for (t=tmin; t<(tmax-0.5*dt); t+=dt) {
			path.curve_to(transform_point(m,f(t)+df(t)*(dt/3.0f)),
			      transform_point(m,f(t+dt) - df(t+dt)*(dt/3.0f)),
			      transform_point(m,f(t+dt)));
		}
		return path;
	}
};

template<typename F>
auto curve(F&& f, float tmin, float tmax, unsigned int nsamples = 100) {
	float dt = (tmax - tmin)/float(nsamples-1); 
	return curve_derivative(std::forward<F>(f), [f,dt] (float t) { return (f(t+0.05f*dt)-f(t))/(0.05f*dt); }, tmin, tmax, nsamples);
}

auto circle(const std::tuple<float,float>& center, float radius, unsigned int nsamples=16) {
	return curve_derivative(
		[center,radius] (float a) { return center + std::tuple<float,float>(radius*std::sin(a),radius*std::cos(a)); },
		[center,radius] (float a) { return std::tuple<float,float>(radius*std::cos(a),-radius*std::sin(a)); }, 0, 2*M_PI, nsamples);
}

}
}
