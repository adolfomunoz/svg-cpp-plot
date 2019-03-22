#pragma once

#include "style.h"

namespace svg_cpp_plot {


//CRTP
//Style attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class StyleAttributes {
public:
	T& class_(const std::string& w) noexcept {
		return static_cast<T*>(this)->set("class",w); 
	}

	T& style(const ElementStyle& w) noexcept {
		static_cast<T*>(this)->set("style",w); 
		return static_cast<T&>(*this);
	}

	ElementStyle& style() {
		return static_cast<T*>(this)->get_or_set("style",ElementStyle());
	}
};

}
