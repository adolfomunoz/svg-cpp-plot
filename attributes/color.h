#pragma once 

#include <sstream>
#include "../core/object.h"
#include <cmath>

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
	rgb(const std::tuple<float,float,float>& c) :
		rgb(std::get<0>(c),std::get<1>(c),std::get<2>(c)) {}

	std::string to_string() const noexcept override {
		std::stringstream s;
		s<<"rgb( "<<int(255.0f*r)<<", "<<int(255.0f*g)<<", "<<int(255.0f*b)<<" )";
		return s.str();
	}
};

class color_hex : public Color {
	std::string hex;
public:
	color_hex(const std::string& hex) : hex(hex) {}
	std::string to_string() const noexcept override {
		return '#'+hex;
	}
};

rgb hsv(float h, float s, float v) {
	h*=3.0f/M_PI; //In 6 sectors
	int i = h; //floor of h;
	float f = h-i; //Decimals part;
	float p = v*(1-s);
	float q = v*(1-s*f);
	float t = v*(1-s*(1-f));
	switch (i){
		case 0: return rgb(v,t,p);
		case 1: return rgb(q,v,p);
		case 2: return rgb(p,v,t);
		case 3: return rgb(p,q,t);
		case 4: return rgb(t,p,v);
		default: //5
			return rgb(v,p,q);
	}
}

//For weird names you only use ocassionaly
class NamedColor : public Color {
    std::string name;
public:
    NamedColor(const std::string_view& s) : name(s) {}
    std::string to_string() const noexcept override { return name; }
};

#define NAMED_COLOR(c)  struct c ## Color : public Color { std::string to_string() const noexcept override { return std::string(#c); } } c;	

NAMED_COLOR(red)
NAMED_COLOR(black)
NAMED_COLOR(white)
NAMED_COLOR(green)
NAMED_COLOR(blue)
NAMED_COLOR(orange)
NAMED_COLOR(yellow)
NAMED_COLOR(cyan)
NAMED_COLOR(magenta)
NAMED_COLOR(pink)
NAMED_COLOR(purple)
NAMED_COLOR(grey)



}
