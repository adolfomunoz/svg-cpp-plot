#pragma once

#include "../attributes/units.h"
#include "../attributes/color.h"
#include "../core/element.h"

namespace svg_cpp_plot {

class Stop : public Terminal, public Attributes<Stop> {
public:
	Stop& offset(float p) noexcept { return this->set("offset",Length(p,percentage)); }
	Stop(float p = 0) : Terminal("stop") { offset(p); }
	float offset() const noexcept { return this->get_default("x",Length(0,percentage)).quantity(); }
	Stop& stop_color(const Color& c) noexcept { return this->set("stop-color", c.to_string()); }
	Stop& stop_opacity(float o) noexcept { return this->set("stop-opacity", o); }
};

class LinearGradient : public NotTerminal, public Attributes<LinearGradient>, public ObjectList<Stop> {
public:
	Length x1() const noexcept { return this->get_default("x1",Length(0)); }
	LinearGradient& x1(const Length& v) noexcept { return this->set("x1",v); }
	LinearGradient& x1(float v) noexcept { return this->x1(Length(v,percentage)); }
	Length x2() const noexcept { return this->get_default("x2",Length(0)); }
	LinearGradient& x2(const Length& v) noexcept { return this->set("x2",v); }
	LinearGradient& x2(float v) noexcept { return this->x2(Length(v,percentage)); }
	Length y1() const noexcept { return this->get_default("y1",Length(0)); }
	LinearGradient& y1(const Length& v) noexcept { return this->set("y1",v); }
	LinearGradient& y1(float v) noexcept { return this->y1(Length(v,percentage)); }
	Length y2() const noexcept { return this->get_default("y2",Length(0)); }
	LinearGradient& y2(const Length& v) noexcept { return this->set("y2",v); }
	LinearGradient& y2(float v) noexcept { return this->y2(Length(v,percentage)); }

	LinearGradient& horizontal() noexcept { return x1(0).x2(100).y1(0).y2(0); }
	LinearGradient& vertical() noexcept { return x1(0).x2(0).y1(0).y2(100); }
	LinearGradient() : NotTerminal("linearGradient") { horizontal(); }
	
	std::string content() const noexcept override {
		return list_to_string("\n");
	}
};

}
