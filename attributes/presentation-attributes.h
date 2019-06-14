#pragma once

#include "color.h"
#include "url.h"
#include "attribute-list.h"
#include "../primitives/mask.h"
#include "../primitives/linear-gradient.h"
#include <initializer_list>

namespace svg_cpp_plot {

/*enum StrokeLinecap { butt, round, square };
namespace detail {
	constexpr const char* stroke_linecap_text[3] = {"butt","round","square"};
}*/


ENUM_TYPE(StrokeLinecap) stroke_linecap_butt("butt"), stroke_linecap_round("round"), stroke_linecap_square("square");

ENUM_TYPE(None) none("none");

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
		return t()->set("fill",n); 
	}
	T& fill(LinearGradient& lg) noexcept {
		return t()->set("fill",url_of(lg));
	}
	T& mask(Mask& m) noexcept {
		return t()->set("mask",url_of(m));
	}
	template<typename G,typename G2>
	T& mask(MaskGenerator<G,G2>& m) noexcept {
		return t()->set("mask",url_of(m));
	}

	T& stroke_linecap(const StrokeLinecap& w) noexcept {
		return t()->set("stroke-linecap",w); 		
	}

	StrokeLinecap stroke_linecap() const noexcept {
		return t()->get_default("stroke-linecap",stroke_linecap_butt);
	}

	T& stroke_dasharray(std::initializer_list<float> l) noexcept {
		return t()->set("stroke-dasharray",AttributeList<float>(l));
	}

	T& stroke_opacity(float v) noexcept {
		return t()->set("stroke-opacity",v);
	}

	T& fill_opacity(float v) noexcept {
		return t()->set("fill-opacity",v);
	}
};

}
