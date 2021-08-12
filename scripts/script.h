#pragma once

#include "../core/element.h"
#include "../attributes/attributes.h"
#include <sstream>
#include <array>
#include <random>

namespace svg_cpp_plot {

class Code : public Object { 
public:
	std::string indented(const std::string& prefix = "    ") const noexcept {
		std::stringstream output;
		std::istringstream code(this->to_string());
		std::string line;
    		while (std::getline(code, line)) output <<prefix<<line << std::endl;
		return output.str();
	}
};

class CodeConstant : public Code {
	std::string code;
public:
	CodeConstant(const std::string& code) : code(code) { }
	std::string to_string() const noexcept override { return code; }
};

std::shared_ptr<CodeConstant> code_from_string(const std::string& code) {
	return std::make_shared<CodeConstant>(code);
}	


std::string random_string(std::string::size_type length = 10) {
    static auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;
    s.reserve(length);

    while(length--) s += chrs[pick(rg)];
    return s;
}

template<std::size_t N>
class CodeFunction : public Code {
    std::string function_name_;
    std::array<std::string,N> param_names_;
public:
    CodeFunction(const std::string& fname = std::string("function_")+random_string()) :
        function_name_(fname) {
        if (N==1) param_names_[0]="param";
        else for(std::size_t i = 0; i<N; ++i) param_names_[i]=std::string("param")+std::to_string(i);
    }
    CodeFunction(const std::string& fname, const std::array<std::string,N>& pnames) :
        function_name_(fname), param_names_(pnames) { }
    CodeFunction(const std::string& fname, const std::initializer_list<std::string>& params) :
        function_name_(fname) {
        auto it = params.begin(); std::size_t i;
        for (i = 0; (it!=params.end()) && (i<N); ++i, ++it) param_names_[i]=(*it);
        for (;i<N;++i) param_names_[i]=std::string("param")+std::to_string(i);
    }
        
     
	virtual std::shared_ptr<Code> apply(const std::array<std::string,N>& params) const noexcept = 0;
	std::shared_ptr<Code> apply(const std::initializer_list<std::string>& params) const  {
        auto it = params.begin();
		std::array<std::string,N> p; std::size_t i;
        for (i = 0; (it!=params.end()) && (i<N); ++i, ++it) p[i]=(*it);
        for (;i<N;++i) p[i]=std::string("param")+std::to_string(i);
		return apply(p);
	}
    
    const std::string& function_name() const { return function_name_; }
    
    std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"function "<<function_name()<<"(";
        int first = true;
        for (std::string p : param_names_) {
            if (first) first = false; else sstr<<", ";
            sstr<<p;
        }
        sstr<<") {"<<std::endl;
        sstr<<this->apply(param_names_)->indented()<<std::endl<<"}"<<std::endl;
	    return sstr.str();	  
    }
};

class Script : public NotTerminal, public Attributes<Style>, public ObjectList<Code> {
public:
	Script() : NotTerminal("script") { set("type","text/javascript"); }

	std::string content() const noexcept override {
		std::stringstream sstr;
		sstr<<"/* <![CDATA[ */"<<std::endl;
		sstr<<list_to_string("\n");
		sstr<<"/* ]]> */"<<std::endl;
	       	return sstr.str();	
	}
};

}
