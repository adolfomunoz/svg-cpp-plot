#pragma once

#include "../primitives/clip-path.h"

namespace svg_cpp_plot {

class unavailable_clip_path : public std::exception {
public: 
	const char* what() const noexcept override { return "Unavailable spot for clip path. Maybe has no parent?"; }
};

template<typename E>
class UrlOf : public Object {
	E& e;
public:
	UrlOf(E& e) : e(e) { }
	std::string to_string() const noexcept override {
		return std::string("url(#")+e.get_default("id",std::string(""))+")";
	}
	E& element() const noexcept { return e; }
};

template<typename E>
UrlOf<E> url_of(E& e) { return UrlOf<E>(e); }

//CRTP
//Style attributes (common to all tags although not used in many of them? I don't know...)
template<typename T>
class GraphicalAttributes {
	constexpr T* t() noexcept { return static_cast<T*>(this); } 
	constexpr const T* t() const noexcept { return static_cast<const T*>(this); } 
public:
	ClipPath& clip_path() {
		if (!t()->parent()) throw unavailable_clip_path();
		else if (!t()->get("clip-path")) return t()->get_or_set("clip-path",url_of(t()->parent()->add(ClipPath()))).element();
		else { ClipPath clip_path; // vv This below is just for getting the data type correctly
		       return t()->get_default("clip-path",url_of(clip_path)).element();
		}
	}
};

}
