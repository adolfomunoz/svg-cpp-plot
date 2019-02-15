#pragma once

namespace svg_cpp_plot {

/*enum TextAnchor { start, middle, end };
enum AlignmentBaseline { hanging, baseline_middle, baseline };
namespace detail {
	constexpr const char* text_anchor_text[3] = {"start","middle","end"};
	constexpr const char* alignment_baseline_text[3] = {"hanging","middle","baseline"};
}*/

ENUM_TYPE(TextAnchor) start("start"), middle("middle"), end("end");
ENUM_TYPE(AlignmentBaseline) hanging("hanging"), baseline_middle("middle"), baseline("baseline");

//CRTP
//Presentation attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class TextPresentationAttributes {
public:
	T& font_size(int s) noexcept {
		return static_cast<T*>(this)->set("font-size",std::to_string(s)+"px"); 
	}

	T& text_anchor(const TextAnchor& a) noexcept {
		return static_cast<T*>(this)->set("text-anchor",a);
	}

	T& alignment_baseline(const AlignmentBaseline& a) noexcept {
		return static_cast<T*>(this)->set("alignment-baseline",a);
	}


	int get_font_size() const noexcept {
		return static_cast<const T*>(this)->get_int("font-size",16);
	}

	TextAnchor get_text_anchor() const noexcept {
		return static_cast<const T*>(this)->get_default("text-anchor",start);
	}

	AlignmentBaseline get_alignment_baseline() const noexcept {
		return static_cast<const T*>(this)->get_default("alignment-baseline",baseline);
	}

};

}
