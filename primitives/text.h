#pragma once 

#include "../core/element.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/text-presentation-attributes.h"
#include "../attributes/style-attributes.h"
#include "../attributes/graphical-attributes.h"
#include "../attributes/units.h"

namespace svg_cpp_plot {

class Text : public NotTerminal, public Attributes<Text>, public GraphicalAttributes<Text>, public StyleAttributes<Text>, public PresentationAttributes<Text>, public TextPresentationAttributes<Text> {
	std::string text_;
public:

	const std::string& text() const noexcept { return text_; }
	Text& text(const std::string& t) noexcept { text_=t; return(*this); }

	std::string content() const noexcept override {
		return text();
	}

	Length x() const noexcept { return get_default("x",Length(0)); }
	Text& x(const Length& v) noexcept { return set("x",v); }
	Length y() const noexcept { return get_default("y",Length(0)); }
	Text& y(const Length& v) noexcept { return set("y",v); }

	Text(float x, float y, const std::string& text) : NotTerminal("text"), text_(text) {
		this->x(x).y(y);
	}
	
	BoundingBox bounding_box() const noexcept override {
		//We account for size and anchor for now. It is not extremelly accurate
		float left = 0; float right = 0;
		float xmeasure = (font_size()*text().size()*2.0)/5.0;
		if (text_anchor() == text_anchor_start)    right = xmeasure; 
		else if (text_anchor() == text_anchor_end) left = xmeasure; 
		else                      right = left = xmeasure/2.0f;
		int top = 0; int bottom = 0;
		float ymeasure = font_size();
		if (dominant_baseline() == dominant_baseline_alphabetic)     { top = ymeasure*3.0/4.0; bottom = ymeasure/4.0; }
		else if (dominant_baseline() == dominant_baseline_hanging) { top = ymeasure/4.0; bottom = ymeasure*3.0/4.0; }
		else                                          { top = ymeasure/2.0; bottom = ymeasure/2.0; }
		return BoundingBox(x()-left,y()-top,x()+right,y()+bottom);
	}
};

}
