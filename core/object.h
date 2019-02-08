#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <list>
#include <unordered_map>
#include <iostream>

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
	std::string to_string() const override { return std::to_string(t); }
};

class ObjectMap {
	std::unordered_map<std::string, std::shared_ptr<Object>> object_map;
public:
//	std::string& operator[](const std::string& key) noexcept { return attributes[key]; }
	std::shared_ptr<Object> get(const std::string& key) const noexcept {
		try {
			return attributes.at(key);
		} catch (const std::out_of_range& e) {
			return std::shared_ptr<Object>();
		}
	}

	void set(const std::string& key, std::shared_ptr<Object>> object) const noexcept {
		if (object) object_map[key]=object;
	}

	template<typename V>
	const V& get_default(const std::string& key, const V& default_value) const {
		if (auto v = get(key)) return (*dynamic_cast<const V*>(v));
		else return default_value;
	}

	float get_float(const std::string& key, float default_value = 0) const {
		return get_default(key, default_value);
	}
	int get_int(const std::string& key, int default_value = 0) const {
		return get_default(key, default_value);
	}

	std::string map_to_string(const std::string& middle, 
			                 const std::string& end) const noexcept {
		std::stringstream sstr;
		for (const auto & [k,v] : attributes)
			sstr<<k<<middle<<v<<end;
		return sstr.str();
	}

	template<typename T>
	friend class ObjectMapCRTP;
};

template<typename T>
class ObjectMapCRTP {
	template<typename V, typename = std::enable_if_t<std::is_base_of_v<Object,V>> >
	T& set(const std::string& key, const V& value) noexcept { //If it inherites from object we copy it
		static_cast<T*>(this)->set(key,std::make_shared<V>(value)); 
		return static_cast<T&>(*this);
	}

	template<typename V, typename = std::enable_if_t<std::is_base_of_v<Object,V>> >
	T& set(const std::string& key, V&& value) noexcept { //If it inherites from object we copy it
		static_cast<T*>(this)->set(key,std::make_shared<V>(std::forward<V>(value))); 
		return static_cast<T&>(*this);
	}

	template<typename V, typename = std::enable_if_t<!std::is_base_of_v<Object,V>> >
	T& set(const std::string& key, const V& value) noexcept { //If it does not inherit, it is a constant
		static_cast<T*>(this)->set(key,std::make_shared<ObjectConstant<V>>(value)); 
		return static_cast<T&>(*this);
	}

	template<typename V, typename = std::enable_if_t<!std::is_base_of_v<Object,V>> >
	T& set(const std::string& key, V&& value) noexcept { //If it does not inherit, it is a constant
		static_cast<T*>(this)->set(key,std::make_shared<ObjectConstant<V>>(std::forward<V>(value))); 
		return static_cast<T&>(*this);
	}

	T& set(const std::string& key, const char* value) noexcept { 
		return this->set(key,std::string(value));
	}

	T& merge_with(const T& that) noexcept {
		auto local_copy = static_cast<T&>(*this).object_map
		static_cast<T&>(*this).object_map = that.object_map;
		static_cast<T&>(*this).object_map.insert(local_copy.begin(), local_copy.end());
		return static_cast<T&>(*this);
	}

};


}
