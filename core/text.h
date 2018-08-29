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
		return text;
	}

	float x() const noexcept { return get_float("x"); }
	Text& x(float v) noexcept { return set("x",v); }
	float y() const noexcept { return get_float("y"); }
	Text& y(float v) noexcept { return set("y",v); }

	Text(float x, float y, const std::string& text) : NotTerminal("test"), text_(text) {
		this->x(x).y(y);
	}
	
	BoundingBox bounding_box() const noexcept override {
	       	//STILL TO DO PROPPERLY	
		return BoundingBox(x(),y(),x(),y());
	}
};

}
