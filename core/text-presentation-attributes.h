#pragma once

namespace svg_cpp_plot {

enum TextAnchor { start, middle, end };
enum AlignmentBaseline { hanging, baseline_middle, baseline };
namespace detail {
	constexpr const char* text_anchor_text[3] = {"start","middle","end"};
	constexpr const char* alignment_baseline_text[3] = {"hanging","middle","baseline"};
}

//CRTP
//Presentation attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class TextPresentationAttributes {
public:
	T& font_size(int s) noexcept {
		return static_cast<T*>(this)->set("font-size",std::to_string(s)+"px"); 
	}

	T& text_anchor(const TextAnchor& a) noexcept {
		return static_cast<T*>(this)->set("text-anchor",detail::text_anchor_text[a]);
	}

	T& alignment_baseline(const AlignmentBaseline& a) noexcept {
		return static_cast<T*>(this)->set("alignment-baseline",detail::alignment_baseline_text[a]);
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

	AlignmentBaseline get_alignment_baseline() const noexcept {
		auto a = static_cast<const T*>(this)->get("alignment-baseline");
		if ((a) && ((*a)=="middle")) return baseline_middle;
		else if ((a) && ((*a)=="hanging")) return hanging;
		else return baseline;
	}

};

}
