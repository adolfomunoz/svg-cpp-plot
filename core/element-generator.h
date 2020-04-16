#pragma once

#include <string>
#include "generator.h"
#include "element.h"

namespace svg_cpp_plot {

template<typename T, typename E>
class ElementGenerator : public Generator<T>, public AttributesBase {
	static_assert(!std::is_abstract_v<E>,"Element type should not be abstract");
	static_assert(std::is_base_of_v<Element,E>,"Element type should inherit from Element");
public:
	virtual E element(const T& t) const noexcept = 0;
	std::string to_string(const T& t) const noexcept override {
		E e = element(t);
		e.merge_map(*this);
		return e.to_string();
	}
};

template<typename T>
class FixedGenerator : public Element {
	T t;
	std::shared_ptr<Generator<T>> gen;

public:
	template<typename E>
	FixedGenerator(const T& t, const E& e) :
		Element("notused"), t(t), gen(std::make_shared<E>(e)) {}

	FixedGenerator(const T& t, const std::shared_ptr<Generator<T>>& g) :
		Element("notused"), t(t), gen(g) {}
	
	std::string to_string() const noexcept override {
		return gen->to_string(t);
	}
};


}
