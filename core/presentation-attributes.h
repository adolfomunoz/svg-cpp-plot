#pragma once

namespace svg_cpp_plot {


//CRTP
//Presentation attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class PresentationAttributes {
public:
	T& stroke_width(const std::string& w) noexcept {
		static_cast<T*>(this)->set("stroke-width",w); 
		return static_cast<T&>(*this);
	}
	T& stroke_width(float w) noexcept {
		return stroke_width(std::to_string(w));
	}
	T& stroke(const std::string& w) noexcept {
		static_cast<T*>(this)->set("stroke",w); 
		return static_cast<T&>(*this);
	}
	T& fill(const std::string& w) noexcept {
		static_cast<T*>(this)->set("fill",w); 
		return static_cast<T&>(*this);
	}
};

}
