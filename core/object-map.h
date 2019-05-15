#pragma once

#include <sstream>
#include <memory>
#include "object.h"
#include <unordered_map>
#include <iostream>
#include <exception>

namespace svg_cpp_plot {

class wrong_object_type : public std::exception {
public: 
	const char* what() const noexcept override { return "Wrong object type in Object Map"; }
};

template<typename O = Object>
class ObjectMap {
	std::unordered_map<std::string, std::shared_ptr<O>> object_map;
	static_assert(std::is_base_of_v<Object,O>,"An object map should be of Object or a subclass of Object");
public:
//	std::string& operator[](const std::string& key) noexcept { return attributes[key]; }
	std::shared_ptr<O> get(const std::string& key) const noexcept {
		try {
			return object_map.at(key);
		} catch (const std::out_of_range& e) {
			return std::shared_ptr<O>();
		}
	}

	template<typename F>
	void for_all(const F& f) {
		for (auto const& i : object_map) f(*(i.second));
	}

	void set_pointer(const std::string& key, std::shared_ptr<O> object) noexcept {
		if (object) object_map[key]=object;
	}

	template<typename V>
	const V& get_default(const std::string& key, const V& default_value) const {
		if (auto v = get(key)) {
			if constexpr (std::is_base_of_v<O,std::decay_t<V>>) { 
				auto ptr_v = std::dynamic_pointer_cast<V>(v);
				if (!ptr_v) throw wrong_object_type();
				else return (*ptr_v);
			}
			else if constexpr (std::is_same_v<O,Object>) {
				auto ptr_v = std::dynamic_pointer_cast<ObjectConstant<std::decay_t<V>>>(v);
				if (!ptr_v) throw wrong_object_type();
				else return ptr_v->value();
			}
			else throw wrong_object_type();
		}
		else return default_value;
	}

	template<typename V> //This is the version for general elements that belong to the inheritance tree
	V& get_or_set(const std::string& key, const V& value, 
			typename std::enable_if<std::is_base_of_v<O,std::decay_t<V>>>::type* = nullptr)
	{
		auto v = get(key);
		if (!v) set_pointer(key,std::make_shared<std::decay_t<V>>(value)); 
		v = get(key); //Should return something, because we have added it above
		auto ptr_v = std::dynamic_pointer_cast<std::decay_t<V>>(v);
		if (!ptr_v) throw wrong_object_type();
		else return (*ptr_v);
	}

	template<typename V> //This is the version for constant values (strings, integers) that can also be added
	const V& get_or_set(const std::string& key, const V& value, 
			typename std::enable_if<(!std::is_base_of_v<O,std::decay_t<V>>) && std::is_same_v<O,Object>>::type* = nullptr)
	{
		auto v = get(key);
		if (!v) set_pointer(key,std::make_shared<ObjectConstant<std::decay_t<V>>>(value)); 
		v = get(key); //Should return something, because we have added it above
		auto ptr_v = std::dynamic_pointer_cast<ObjectConstant<std::decay_t<V>>>(v);
		if (!ptr_v) throw wrong_object_type();
		else return ptr_v->value();
	}

	void merge_map(const ObjectMap<O>& that) {
		auto local_copy = object_map;
		object_map = that.object_map;
		object_map.insert(local_copy.begin(), local_copy.end());
	}


/*
	template<typename V>
	V& get_or_set(const std::string& key, const V& value) {
		auto v = get(key);
		if (!v) {
			if constexpr (std::is_base_of_v<O,std::decay_t<V>>)
				set_pointer(key,std::make_shared<std::decay_t<V>>(value)); 
			else if constexpr (std::is_same_v<Object,O>) //Otherwise it is a constant (if these are objects)
				set_pointer(key,std::make_shared<ObjectConstant<std::decay_t<V>>>(value)); 
			else throw wrong_object_type();
		}
		v = get(key); //Should return something, because we have added it above
		if constexpr (std::is_base_of_v<O,std::decay_t<V>>) { 
			auto ptr_v = std::dynamic_pointer_cast<V>(v);
			if (!ptr_v) throw wrong_object_type();
			else return (*ptr_v);
		}
		else if constexpr (std::is_same_v<O,Object>) {
			auto ptr_v = std::dynamic_pointer_cast<ObjectConstant<std::decay_t<V>>>(v);
			if (!ptr_v) throw wrong_object_type();
			else return ptr_v->value();
		}
		else throw wrong_object_type();
	}


	template<typename V>
	V& get_or_set(const std::string& key, V&& value) {
		if (auto v = get(key)) {
			if constexpr (std::is_base_of_v<O,std::decay_t<V>>) { 
				auto ptr_v = std::dynamic_pointer_cast<V>(v);
				if (!ptr_v) throw wrong_object_type();
				else return (*v);
			}
			else if constexpr (std::is_same_v<O,Object>) {
				auto ptr_v = std::dynamic_pointer_cast<ObjectConstant<std::decay_t<V>>>(v);
				if (!ptr_v) throw wrong_object_type();
				else return v->value();
			}
			else throw wrong_object_type();
		}
		else {
			if constexpr (std::is_base_of_v<Object,std::decay_t<V>>)
				set_pointer(key,std::make_shared<std::decay_t<V>>(std::forward<V>(value))); 
			else if constexpr (std::is_same_v<Object,O>) //Otherwise it is a constant (if these are objects)
				set_pointer(key,std::make_shared<ObjectConstant<std::decay_t<V>>>(std::forward<V>(value))); 
			else throw wrong_object_type();
			return get_default(key, std::forward<V>(value));
		}
	}*/




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

	template<typename T, typename O2>
	friend class ObjectMapCRTP;
};

template<typename T, typename O = Object>
class ObjectMapCRTP {
public:
	template<typename V>
	T& set(const std::string& key, const V& value) noexcept { 
		//If it inherites from object we copy it
		if constexpr (std::is_base_of_v<O,std::decay_t<V>>)
			static_cast<T*>(this)->set_pointer(key,std::make_shared<std::decay_t<V>>(value)); 
		else if constexpr (std::is_same_v<Object,O>) //Otherwise it is a constant (if these are objects)
			static_cast<T*>(this)->set_pointer(key,std::make_shared<ObjectConstant<std::decay_t<V>>>(value)); 
		else throw wrong_object_type();
		return static_cast<T&>(*this);
	}



	template<typename V>
	T& set(const std::string& key, V&& value) noexcept { 		//If it inherites from object we copy it
		if constexpr (std::is_base_of_v<Object,std::decay_t<V>>)
			static_cast<T*>(this)->set_pointer(key,std::make_shared<std::decay_t<V>>(std::forward<V>(value))); 
		else if constexpr (std::is_same_v<Object,O>) //Otherwise it is a constant (if these are objects)
			static_cast<T*>(this)->set_pointer(key,std::make_shared<ObjectConstant<std::decay_t<V>>>(std::forward<V>(value))); 
		else throw wrong_object_type();
		return static_cast<T&>(*this);
	}




	T& set(const std::string& key, const char* value) noexcept { 
		static_cast<T*>(this)->set_pointer(key,std::make_shared<ObjectConstant<std::string>>(std::string(value)));
		return static_cast<T&>(*this);
	}

	T& merge_with(const T& that) noexcept {
		static_cast<T&>(*this).merge_map(that);
		return static_cast<T&>(*this);
	}

};




}
