#pragma once 

#include <sstream>
#include "../core/object.h"

namespace svg_cpp_plot {

class Color : public Object { };

//Between 0.0 and 1.0, we make the conversion internally
class rgb : public Color {
	float r, g, b;
public:
	rgb(float r, float g, float b) : 
		r(std::max(std::min(r,1.0f),0.0f)), 
		g(std::max(std::min(g,1.0f),0.0f)),
	       	b(std::max(std::min(b,1.0f),0.0f)) { }

	std::string to_string() const noexcept override {
		std::stringstream s;
		s<<"rgb( "<<int(255.0f*r)<<", "<<int(255.0f*g)<<", "<<int(255.0f*b)<<" )";
		return s.str();
	}
};

#define NAMED_COLOR(c)  struct c ## Color : public Color { std::string to_string() const noexcept override { return std::string(#c); } } c;	

NAMED_COLOR(red)
NAMED_COLOR(black)
NAMED_COLOR(white)
NAMED_COLOR(green)
NAMED_COLOR(blue)
NAMED_COLOR(orange)
NAMED_COLOR(yellow)
NAMED_COLOR(pink)
NAMED_COLOR(purple)
NAMED_COLOR(grey)



}
