#pragma once

#include <sstream>
#include <memory>
#include "object.h"
#include <list>
#include <iostream>

namespace svg_cpp_plot {

template<typename O = Object>
class ObjectList {
	static_assert(std::is_base_of_v<Object,O>,"Should inherit from object");
protected:
	virtual void on_add(O& o) {}

	std::list<std::shared_ptr<O>> object_list;
	
	O& add_ptr(const std::shared_ptr<O>& o) {
		object_list.push_back(o); 
		on_add(*object_list.back());
		return (*object_list.back());
	}
public:
	//Add directly the pointer (instancing?)
	O& add(const std::shared_ptr<O>& o) {
		return add_ptr(o);
	}

	//Add by copy
	template<typename T> 
	T& add(const T& t) {
		return static_cast<T&>(add_ptr(std::make_shared<std::decay_t<T>>(t)));
	}

	//Add by move if possible
	template<typename T> 
	T& add(T&& t) {
		return static_cast<T&>(add_ptr(std::make_shared<std::decay_t<T>>(std::forward<T>(t))));
	}

	template<typename P>
	void remove_if(const P& p) {
		object_list.remove_if([&p] (const std::shared_ptr<O>& o) { return p(*o); });
	}
    
    template<typename T>
    bool contains(const T& t) {
        for (auto o : object_list) if ((*o) == t) return true;
        return false;
    }

	std::string list_to_string(const std::string& sep) const noexcept {
		std::stringstream sstr;
		if (!object_list.empty()) {
			auto it = object_list.begin();
			sstr<<(*it)->to_string(); ++it;
			while (it != object_list.end()) { sstr<<sep<<(*it)->to_string(); ++it; }
		}
		return sstr.str();
	}
};



}
