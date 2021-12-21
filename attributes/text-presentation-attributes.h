#pragma once

#include "units.h"

namespace svg_cpp_plot {

/*enum TextAnchor { start, middle, end };
enum AlignmentBaseline { hanging, baseline_middle, baseline };
namespace detail {
	constexpr const char* text_anchor_text[3] = {"start","middle","end"};
	constexpr const char* alignment_baseline_text[3] = {"hanging","middle","baseline"};
}*/

ENUM_TYPE(TextAnchor) text_anchor_start("start"), text_anchor_middle("middle"), text_anchor_end("end");
ENUM_TYPE(AlignmentBaseline) alignment_baseline_hanging("hanging"), alignment_baseline_middle("middle"), alignment_baseline("baseline");
ENUM_TYPE(DominantBaseline) dominant_baseline_hanging("hanging"), dominant_baseline_middle("middle"), dominant_baseline_center("center"), dominant_baseline_alphabetic("alphabetic"), dominant_baseline_mathematical("mathematical");


//CRTP
//Presentation attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class TextPresentationAttributes {
	constexpr T* t() noexcept { return static_cast<T*>(this); } 
	constexpr const T* t() const noexcept { return static_cast<const T*>(this); } 
public:
	T& font_size(const Length& s) noexcept {
		return t()->set("font-size",s); 
	}

	T& text_anchor(const TextAnchor& a) noexcept {
		return t()->set("text-anchor",a);
	}

	T& alignment_baseline(const AlignmentBaseline& a) noexcept {
		return t()->set("alignment-baseline",a);
	}

	T& dominant_baseline(const DominantBaseline& b) noexcept {
		return t()->set("dominant-baseline",b);
	}

	Length font_size() const noexcept {
		return t()->get_default("font-size",Length(16,px));
	}

	TextAnchor text_anchor() const noexcept {
		return t()->get_default("text-anchor",text_anchor_start);
	}

	AlignmentBaseline alignment_baseline() const noexcept {
		return t()->get_default("alignment-baseline",svg_cpp_plot::alignment_baseline);
	}

	DominantBaseline dominant_baseline() const noexcept {
		return t()->get_default("dominant-baseline",dominant_baseline_alphabetic);
	}


};

}
