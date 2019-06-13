#pragma once 

#include "../core/object.h"

namespace svg_cpp_plot {

class LengthUnit {
	std::string name_;
	float ratio_; //With respect to pixels. Not useful with percentage (we invent a number there that does not work...)
public:
	LengthUnit(const std::string& name, float ratio) :
		name_(name), ratio_(ratio) { }
	const std::string& name() const noexcept { return name_; }
	float ratio() const noexcept { return ratio_; }
} px("px",1), cm("cm",37.795), mm("mm",3.7795), in("in", 96), pc("pc", 16), percentage("%",1024);



class Length : public Object {
	float q;
	LengthUnit unit;
public:
	Length(float quantity, const LengthUnit& unit = px) : q(quantity), unit(unit) { }
	float quantity() const { return q; }
	std::string to_string() const noexcept override { return std::to_string(quantity())+unit.name(); }
	operator float() const noexcept { return quantity()*unit.ratio(); }
};
}
