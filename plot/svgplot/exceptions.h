#pragma once

#include <string>
#include <typeinfo>

#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#endif

namespace svg_cpp_plot {
    
class exception : public std::exception {
    using std::exception::exception;
    
};

class exception_invalid_parameter : public exception {
    std::string parameter;
    std::string type;
    std::string msg;

#ifdef __GNUG__
    static std::string demangle(const char* name) {
        int status = -4; // some arbitrary value to eliminate the compiler warning
        // enable c++11 by passing the flag -std=c++11 to g++
        std::unique_ptr<char, void(*)(void*)> res {
            abi::__cxa_demangle(name, NULL, NULL, &status),
            std::free
        };
        return (status==0) ? res.get() : name ;
    }
#else
    // does nothing if not g++
    std::string demangle(const char* name) {
        return name;
    }
#endif

public:
    template<typename T>
    exception_invalid_parameter(const T& t, const std::string& p) :
        parameter(p), type(demangle(typeid(t).name())), msg(demangle(typeid(t).name()) + std::string(" has not implemented the ") + p + "named parameter")  {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class exception_unknown_value : public exception {
    std::string parameter;
    std::string value;
    std::string msg;
public:
    exception_unknown_value(const std::string& p, const std::string& v) :
        parameter(p), value(v), msg(std::string("The parameter ")+p+" cannot have the value "+v) {}
    const char* what() const noexcept override { return msg.c_str(); }
};
  
};