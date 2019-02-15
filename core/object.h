#pragma once

#include <string>

namespace svg_cpp_plot {

class Object {
public:
	virtual std::string to_string() const noexcept = 0;
};

template<typename T> 
class ObjectConstant : public Object {
	T t;
public:
	ObjectConstant(const T& t) : t(t) { }
	ObjectConstant(T&& t) : t(std::forward<T>(t)) { }
	std::string to_string() const noexcept override { return std::to_string(t); }
	const T& value() const noexcept { return t; }
};

template<> 
class ObjectConstant<std::string> : public Object {
	std::string t;
public:
	ObjectConstant(const std::string& t) : t(t) { }
	ObjectConstant(std::string&& t) : t(std::forward<std::string>(t)) { }
	std::string to_string() const noexcept override { return t; }
	bool operator==(const ObjectConstant<std::string>& that) { return this->t == that.t; }
	const std::string& value() const noexcept { return t; }
};

#define ENUM_TYPE(T) \
class T : public ObjectConstant<std::string> { \
public: using ObjectConstant<std::string>::ObjectConstant; \
}


}
