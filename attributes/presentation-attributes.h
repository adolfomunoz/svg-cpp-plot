#pragma once

#include "color.h"
#include "attribute-list.h"
#include <initializer_list>

namespace svg_cpp_plot {

/*enum StrokeLinecap { butt, round, square };
namespace detail {
	constexpr const char* stroke_linecap_text[3] = {"butt","round","square"};
}*/


ENUM_TYPE(StrokeLinecap) stroke_linecap_butt("butt"), stroke_linecap_round("round"), stroke_linecap_square("square");

struct None {} none;

//CRTP
//Presentation attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class PresentationAttributes {
	constexpr T* t() noexcept { return static_cast<T*>(this); } 
	constexpr const T* t() const noexcept { return static_cast<const T*>(this); } 
public:
	T& stroke_width(const std::string& w) noexcept {
		return t()->set("stroke-width",w); 
	}
	T& stroke_width(float w) noexcept {
		return stroke_width(std::to_string(w));
	}
	T& stroke(const Color& c) noexcept {
		return t()->set("stroke",c.to_string()); 
	}
	T& fill(const Color& c) noexcept {
		return t()->set("fill",c.to_string()); 
	}
	T& fill(const None& n) noexcept {
		return t()->set("fill","none"); 
	}

	T& stroke_linecap(const StrokeLinecap& w) noexcept {
		return t()->set("stroke_linecap",w); 		
	}

	StrokeLinecap stroke_linecap() const noexcept {
		return t()->get_default("stroke_linecap",stroke_linecap_butt);
	}

	T& stroke_dasharray(std::initializer_list<float> l) noexcept {
		return t()->set("stroke-dasharray",AttributeList<float>(l));
	}
};

}
