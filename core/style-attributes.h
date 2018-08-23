#pragma once

#include "style.h"

namespace svg_cpp_plot {


//CRTP
//Style attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class StyleAttributes {
public:
	T& class_(const std::string& w) noexcept {
		static_cast<T*>(this)->set("class",w); 
		return static_cast<T&>(*this);
	}
	T& style(const StyleEntry& w) noexcept {
		StyleEntry n(w); 
		static_cast<T*>(this)->set("style",n.id("").to_string()); 
		return static_cast<T&>(*this);
	}
};

}
