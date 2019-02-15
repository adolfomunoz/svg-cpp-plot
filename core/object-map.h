#pragma once

#include <sstream>
#include <memory>
#include "object.h"
#include <unordered_map>
#include <iostream>

namespace svg_cpp_plot {

class ObjectMap {
	std::unordered_map<std::string, std::shared_ptr<Object>> object_map;
public:
//	std::string& operator[](const std::string& key) noexcept { return attributes[key]; }
	std::shared_ptr<Object> get(const std::string& key) const noexcept {
		try {
			return object_map.at(key);
		} catch (const std::out_of_range& e) {
			return std::shared_ptr<Object>();
		}
	}

	void set_pointer(const std::string& key, std::shared_ptr<Object> object) noexcept {
		if (object) object_map[key]=object;
	}

	template<typename V>
	const V& get_default(const std::string& key, const V& default_value) const {
		if (auto v = get(key)) {
			if constexpr (std::is_base_of_v<Object,std::decay_t<V>>) 
				return (*std::dynamic_pointer_cast<V>(v));
			else
				return std::dynamic_pointer_cast<ObjectConstant<std::decay_t<V>>>(v)->value();
		}
		else return default_value;
	}

	//vv Not necessary, backwards compatibility
	float get_float(const std::string& key, float default_value = 0) const {
		return get_default(key, default_value);
	}
	//vv Not necessary, backwards compatibility
	int get_int(const std::string& key, int default_value = 0) const {
		return get_default(key, default_value);
	}

	std::string map_to_string(const std::string& middle, 
			                 const std::string& end) const noexcept {
		std::stringstream sstr;
		for (const auto & [k,v] : object_map)
			sstr<<k<<middle<<v->to_string()<<end;
		return sstr.str();
	}

	template<typename T>
	friend class ObjectMapCRTP;
};

template<typename T>
class ObjectMapCRTP {
public:
	template<typename V>
	T& set(const std::string& key, const V& value) noexcept { 
		//If it inherites from object we copy it
		if constexpr (std::is_base_of_v<Object,std::decay_t<V>>)
			static_cast<T*>(this)->set_pointer(key,std::make_shared<std::decay_t<V>>(value)); 
		else //Otherwise it is a constant
			static_cast<T*>(this)->set_pointer(key,std::make_shared<ObjectConstant<std::decay_t<V>>>(value)); 
		return static_cast<T&>(*this);
	}

	template<typename V>
	T& set(const std::string& key, V&& value) noexcept { 		//If it inherites from object we copy it
		if constexpr (std::is_base_of_v<Object,std::decay_t<V>>)
			static_cast<T*>(this)->set_pointer(key,std::make_shared<std::decay_t<V>>(std::forward<V>(value))); 
		else //Otherwise it is a constant
			static_cast<T*>(this)->set_pointer(key,std::make_shared<ObjectConstant<std::decay_t<V>>>(std::forward<V>(value))); 
		return static_cast<T&>(*this);
	}

	T& set(const std::string& key, const char* value) noexcept { 
		static_cast<T*>(this)->set_pointer(key,std::make_shared<ObjectConstant<std::string>>(std::string(value)));
		return static_cast<T&>(*this);
	}

	T& merge_with(const T& that) noexcept {
		auto local_copy = static_cast<T&>(*this).object_map;
		static_cast<T&>(*this).object_map = that.object_map;
		static_cast<T&>(*this).object_map.insert(local_copy.begin(), local_copy.end());
		return static_cast<T&>(*this);
	}

};




}
