#pragma once 

#include "element.h"
#include "presentation-attributes.h"
#include "text-presentation-attributes.h"
#include "style-attributes.h"

namespace svg_cpp_plot {

class Text : public NotTerminal, public Attributes<Text>, public StyleAttributes<Text>, public PresentationAttributes<Text>, public TextPresentationAttributes<Text> {
	std::string text_;
public:

	const std::string& text() const noexcept { return text_; }
	Text& text(const std::string& t) noexcept { text_=t; return(*this); }

	std::string content() const noexcept override {
		return text();
	}

	float x() const noexcept { return get_float("x"); }
	Text& x(float v) noexcept { return set("x",v); }
	float y() const noexcept { return get_float("y"); }
	Text& y(float v) noexcept { return set("y",v); }

	Text(float x, float y, const std::string& text) : NotTerminal("text"), text_(text) {
		this->x(x).y(y);
	}
	
	BoundingBox bounding_box() const noexcept override {
		//We account for size and anchor for now. It is not extremelly accurate
		float left = 0; float right = 0;
		float xmeasure = (get_font_size()*text().size()*2.0)/5.0;
		if (get_text_anchor() == start)    right = xmeasure; 
		else if (get_text_anchor() == end) left = xmeasure; 
		else                               right = left = xmeasure/2.0f;
		int top = 0; int bottom = 0;
		float ymeasure = get_font_size();
		if (get_alignment_baseline() == baseline)     { top = ymeasure*3.0/4.0; bottom = ymeasure/4.0; }
		else if (get_alignment_baseline() == hanging) { top = ymeasure/4.0; bottom = ymeasure*3.0/4.0; }
		else                                          { top = ymeasure/2.0; bottom = ymeasure/2.0; }
		return BoundingBox(x()-left,y()-top,x()+right,y()+bottom);
	}
};

}