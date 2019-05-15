#pragma once

#include <string>
#include "generator.h"
#include "element.h"

namespace svg_cpp_plot {

template<typename T, typename O>
class ElementGenerator : public Generator<T>, public AttributesBase {
	static_assert(!std::is_abstract_v<E>,"Element type should not be abstract");
	static_assert(std::is_base_of<Element,E>,"Element type should inherit from Element");
public:
	virtual E element(const T& t) const noexcept = 0;
	std::string to_string(const T& t) const noexcept override {
		E e = element(t);
		e.merge_map(*this);
		return e.to_string();
	}
};

}
