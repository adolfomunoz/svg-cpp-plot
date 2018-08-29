#pragma once

namespace svg_cpp_plot {

namespace TextAnchor {
	constexpr const char* text[3] = {"start","middle","end"};
	enum Value { START, MIDDLE, END };
};
//CRTP
//Presentation attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class TextPresentationAttributes {
public:
	T& text_anchor(const TextAnchor::Value& a) noexcept {
		return static_cast<T*>(this)->set("text-anchor",TextAnchor::text[a]);
	}
};

}
