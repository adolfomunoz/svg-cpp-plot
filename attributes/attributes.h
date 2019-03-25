#pragma once

#include "../core/object-map.h"

namespace svg_cpp_plot {

using AttributesBase = ObjectMap<Object>;

//CRTP
template<typename T>
class Attributes : public ObjectMapCRTP<T> {
public:
	template<typename V>
	T& set_custom(const std::string& key, const V& value) noexcept { 
		return this->set(std::string("data-")+key, value);
	}

	template<typename V>
	T& set_custom(const std::string& key, V&& value) noexcept { 
		return this->set(std::string("data-")+key, std::forward<V>(value));
	}
	
	T& id(const std::string& value) noexcept {
		return this->set("id",value);
	}

	std::string id() const noexcept {
		return static_cast<const T*>(this)->get_default("id",std::string());
	}

	T& lang(const std::string& value) noexcept {
		return this->set("lang",value);
	}
	
	T& tabindex(const std::string& value) noexcept {
		return this->set("tabindex",value);
	}
}; 

}

