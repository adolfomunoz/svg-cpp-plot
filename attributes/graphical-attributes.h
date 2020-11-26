#pragma once

#include "../primitives/clip-path.h"
#include "url.h"

namespace svg_cpp_plot {

class unavailable_clip_path : public std::exception {
public: 
	const char* what() const noexcept override { return "Unavailable spot for clip path. Maybe has no parent?"; }
};

class Transform : public Object {};

class matrix : public Transform {
	float a, b, c, d, e, f;
public:
	matrix(float a, float b, float c, float d, float e, float f) :
		a(a), b(b), c(c), d(d), e(e), f(f) { }
	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"matrix("<<a<<" "<<b<<" "<<c<<" "<<d<<" "<<e<<" "<<f<<")";
		return sstr.str();
	}
};

class Translate : public Transform {
	float x,y;
public:
	Translate(float x, float y=0) :
		x(x), y(y) { }
	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"translate("<<x<<" "<<y<<")";
		return sstr.str();
	}
};

std::shared_ptr<Transform> translate(float x, float y=0) { return std::make_shared<Translate>(x,y); }

class Scale : public Transform {
	float x,y;
public:
	Scale(float x, float y) :
		x(x), y(y) { }
	Scale(float x) : Scale(x,x) {}

	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"scale("<<x<<" "<<y<<")";
		return sstr.str();
	}
};

std::shared_ptr<Transform> scale(float x) { return std::make_shared<Scale>(x); }
std::shared_ptr<Transform> scale(float x, float y) { return std::make_shared<Scale>(x,y); }

class Rotate : public Transform {
	float a,x,y;
public:
	Rotate(float a, float x, float y) :
		a(a), x(x), y(y) { }
	Rotate(float a) : Rotate(a,0,0) {}

	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"rotate("<<a<<" "<<x<<" "<<y<<")";
		return sstr.str();
	}
};

std::shared_ptr<Transform> rotate(float a) { return std::make_shared<Rotate>(a); }
std::shared_ptr<Transform> rotate(float a, float x, float y) { return std::make_shared<Rotate>(a,x,y); }

ENUM_TYPE(VectorEffect) vector_effect_default("default"), non_scaling_stroke("non-scaling-stroke");
ENUM_TYPE(PointerEvents) pointer_events_auto("auto"), pointer_events_none("none"), pointer_events_visible_painted("visiblePainted"), pointer_events_visible_fill("visibleFill"), pointer_events_visible_stroke("visibleStroke"), pointer_events_painted("painted"), pointer_events_fill("fill"), pointer_events_stroke("stroke"), pointer_events_all("all");  







template<typename T>
struct clip_path_obtainer {
	T& t;
public:
	clip_path_obtainer(T& t) : t(t) {}
	ClipPath& clip_path() {
		if (!t->parent()) throw unavailable_clip_path();
		else if (!t->get("clip-path")) return t->get_or_set("clip-path",url_of(t->parent()->add(ClipPath()))).element();
		else { ClipPath clip_path; // vv This below is just for getting the data type correctly
		       return t->get_default("clip-path",url_of(clip_path)).element();
		}
	}
};
//CRTP
//Style attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class GraphicalAttributes {
	constexpr T* t() noexcept { return static_cast<T*>(this); } 
	constexpr const T* t() const noexcept { return static_cast<const T*>(this); } 
public:
	auto& clip_path() { return clip_path_obtainer(t()).clip_path(); }
	
	T& transform(const AttributeList<Transform>& tr) noexcept {
		return t()->set("transform",tr);
	}

	template<typename Q, typename... Qs>
	T& transform(std::tuple<Q,Qs...> tr) noexcept {
		return this->transform(AttributeList<Transform>(tr));
	}

	AttributeList<Transform>& transform() {
		return t()->get_or_set("transform",AttributeList<Transform>());
	}

	T& opacity(float f) noexcept {
		return t()->set("opacity",f);
	}

	T& vector_effect(const VectorEffect& ve) noexcept {
		return t()->set("vector-effect",ve);
	}

	T& pointer_events(const PointerEvents& pe) noexcept {
		return t()->set("pointer-events",pe);
	}
};

}
