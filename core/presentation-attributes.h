#pragma once

#include "color.h"

namespace svg_cpp_plot {

enum StrokeLinecap { butt, round, square };
namespace detail {
	constexpr const char* stroke_linecap_text[3] = {"butt","round","square"};
}

struct None {} none;

//CRTP
//Presentation attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class PresentationAttributes {
public:
	T& stroke_width(const std::string& w) noexcept {
		return static_cast<T*>(this)->set("stroke-width",w); 
	}
	T& stroke_width(float w) noexcept {
		return stroke_width(std::to_string(w));
	}
	T& stroke(const Color& c) noexcept {
		return static_cast<T*>(this)->set("stroke",c.to_string()); 
	}
	T& fill(const Color& c) noexcept {
		return static_cast<T*>(this)->set("fill",c.to_string()); 
	}
	T& fill(const None& n) noexcept {
		return static_cast<T*>(this)->set("fill","none"); 
	}

	T& stroke_linecap(const StrokeLinecap& w) noexcept {
		return static_cast<T*>(this)->set("stroke-linecap",detail::stroke_linecap_text[w]); 		
	}
};

}
