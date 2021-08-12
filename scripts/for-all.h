#pragma once

#include "script.h"

namespace svg_cpp_plot {
namespace script {
class ForAll : public Code {
	std::string selector;
	std::shared_ptr<CodeFunction<1>> code;
public:
	ForAll(const std::string& selector, std::shared_ptr<CodeFunction<1>> code) : selector(selector), code(code) { }
	template<typename C, typename = std::enable_if_t<std::is_base_of_v<CodeFunction<1>,C>> >
	ForAll(const std::string& selector, const C& code) : ForAll(selector,std::make_shared<C>(code)) { }

	std::string to_string() const noexcept override {
		std::string param = std::string("param")+random_string();
		std::stringstream output;
		output << "document.querySelectorAll(\"" << selector <<"\").forEach(function("<<param<<") {"<<std::endl;
		output << code->apply({param})->indented()<< std::endl;
		output << "});"<<std::endl;
		return output.str();
	}
};	

}
}
