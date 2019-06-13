#pragma once 

#include "../core/object.h"

namespace svg_cpp_plot {

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

}
