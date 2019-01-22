#pragma once 

#include "object.h"
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
		int left = 0; int right = 0;
		int measure = (get_font_size()*text().size()*2)/5;
		switch (get_text_anchor()) {
			case start: right = measure; break;
			case end: left  = measure; break;
			default: right = left = measure/2; break;
		}
		return BoundingBox(x()-left,y()-(get_font_size()*3)/4,x()+right,y()+get_font_size()/4);
	}
};

}
