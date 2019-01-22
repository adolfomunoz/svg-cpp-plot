#pragma once

#include <unordered_map>
#include <sstream>
#include <string>

namespace svg_cpp_plot {

class AttributesBase {
	std::unordered_map<std::string, std::string> attributes;
public:
	std::string& operator[](const std::string& key) noexcept { return attributes[key]; }
	std::optional<std::string> get(const std::string& key) const noexcept {
		try {
			return attributes.at(key);
		} catch (const std::out_of_range& e) {
			return {};
		}
	}
	float get_float(const std::string& key, float default_value = 0) const noexcept {
		if (auto v = get(key)) return stof(v.value()); 
		else return default_value;
	}
	int get_int(const std::string& key, int default_value = 0) const noexcept {
		if (auto v = get(key)) return stoi(v.value()); 
		else return default_value;
	}

	std::string attributes_to_string(const std::string& middle, 
			                 const std::string& end) const noexcept {
		std::stringstream sstr;
		for (const auto & [k,v] : attributes)
			sstr<<k<<middle<<v<<end;
		return sstr.str();
	}
};

//CRTP
template<typename T>
class Attributes {
public:
	template<typename V>
	T& set(const std::string& key, const V& value) noexcept { 
		static_cast<T&>(*this)[key] = std::to_string(value); return static_cast<T&>(*this);
	}

	T& set(const std::string& key, const char* value) noexcept { 
		static_cast<T&>(*this)[key] = std::string(value); return static_cast<T&>(*this);
	}

	T& set(const std::string& key, const std::string& value) noexcept { 
		static_cast<T&>(*this)[key] = value; return static_cast<T&>(*this);
	}
	
	template<typename V>
	T& set_custom(const std::string& key, const V& value) noexcept { 
		static_cast<T&>(*this)[key] = std::string("data-")+std::to_string(value); 
		return static_cast<T&>(*this);
	}

	T& set_custom(const std::string& key, const char* value) noexcept { 
		static_cast<T&>(*this)[key] = std::string("data-")+std::string(value); return static_cast<T&>(*this);
	}

	T& set_custom(const std::string& key, const std::string& value) noexcept { 
		static_cast<T&>(*this)[key] = std::string("data-")+value; return static_cast<T&>(*this);
	}
	
	T& id(const std::string& value) noexcept {
		return set("id",value);
	}

	T& lang(const std::string& value) noexcept {
		return set("lang",value);
	}
	
	T& tabindex(const std::string& value) noexcept {
		return set("tabindex",value);
	}
}; 

}

