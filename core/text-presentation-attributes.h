#pragma once

namespace svg_cpp_plot {

enum TextAnchor { start, middle, end };
namespace detail {
	constexpr const char* text_anchor_text[3] = {"start","middle","end"};
}

//CRTP
//Presentation attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class TextPresentationAttributes {
public:
	T& font_size(int s) noexcept {
		return static_cast<T*>(this)->set("font-size",s); 
	}

	T& text_anchor(const TextAnchor& a) noexcept {
		return static_cast<T*>(this)->set("text-anchor",detail::text_anchor_text[a]);
	}

	int get_font_size() const noexcept {
		return static_cast<const T*>(this)->get_int("font-size",16);
	}

	TextAnchor get_text_anchor() const noexcept {
		auto a = static_cast<const T*>(this)->get("text-anchor");
		if ((a) && ((*a)=="middle")) return middle;
		else if ((a) && ((*a)=="end")) return end;
		else return start;
	}
};

}
