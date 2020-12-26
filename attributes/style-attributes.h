#pragma once

#include "style.h"

namespace svg_cpp_plot {


//CRTP
//Style attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class StyleAttributes {
	constexpr T* t() noexcept { return static_cast<T*>(this); } 
	constexpr const T* t() const noexcept { return static_cast<const T*>(this); } 
public:
	T& class_(const AttributeList<std::string>& w) noexcept {
		return t()->set("class",w); 
	}

	T& class_(const char* w) noexcept {
		return class_({std::string(w)});
	}

	AttributeList<std::string>& class_() noexcept {
		return t()->get_or_set("class",AttributeList<std::string>());
	}
    
    AttributeList<std::string> class_() const noexcept {
		return t()->get_default("class",AttributeList<std::string>());
	}

	T& style(const ElementStyle& w) noexcept {
		return t()->set("style",w); 
	}

	ElementStyle& style() noexcept {
		return t()->get_or_set("style",ElementStyle());
	}
    
    ElementStyle style() const noexcept {
		return t()->get_default("style",ElementStyle());     
    }
    
};

}
