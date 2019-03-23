#pragma once

namespace svg_cpp_plot {


//At some point we might need percentages, sum types or different kinds of units for lengths and positions, but we ignore it for now (and maybe forever).

//CRTP
//GeometryAttributes for geometrical elements
template<typename T>
class GeometryAttributes {
	constexpr T* t() noexcept { return static_cast<T*>(this); } 
	constexpr const T* t() const noexcept { return static_cast<const T*>(this); } 
public:
	float x() const noexcept { return t()->get_float("x"); }
	T& x(float v) noexcept { return t()->set("x",v); }
	float y() const noexcept { return t()->get_float("y"); }
	T& y(float v) noexcept { return t()->set("y",v); }
	float height() const noexcept { return t()->get_float("height"); }
	T& height(float v) noexcept { return t()->set("height",v); }
	float width() const noexcept { return t()->get_float("width"); }
	T& width(float v) noexcept { return t()->set("width",v); }

	T& rect(float x1, float y1, float x2, float y2) noexcept {
		return x(std::min(x1,x2)).y(std::min(y1,y2)).width(std::abs(x2-x1)).height(std::abs(y2-y1));
	}
	template<typename P, typename = std::enable_if_t<is_2d_point_v<P>> >
	T& rect(const P& p1, const P& p2) noexcept {
		return rect(std::get<0>(p1),std::get<1>(p1),std::get<0>(p2),std::get<1>(p2));
	}

};

}
