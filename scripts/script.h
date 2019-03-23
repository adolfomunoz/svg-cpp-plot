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

template<std::size_t N>
class CodeFunction {
public:
	virtual std::shared_ptr<Code> apply(const std::array<std::string,N>& params) const noexcept = 0;
	std::shared_ptr<Code> apply(const std::initializer_list<std::string>& params) const  {
		//We don't check for the number of parameters, we should.
		std::array<std::string,N> p;
		std::copy(params.begin(),params.end(),p.begin());
		return apply(p);
	}
};

std::string random_parameter_name(std::string::size_type length = 10)
{
    static auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s("param");

    s.reserve(length+5);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}

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
