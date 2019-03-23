#pragma once

#include "script.h"

namespace svg_cpp_plot {
namespace script {
class SetTimeout : public Code {
	std::shared_ptr<Code> code;
	float time; //In seconds;
public:
	SetTimeout(std::shared_ptr<Code> code, float time) : code(code), time(time) { }
	template<typename C, typename = std::enable_if_t<std::is_base_of_v<Code,C>> >
	SetTimeout(const C& code, float time) : SetTimeout(std::make_shared<C>(code),time) { }

	std::string to_string() const noexcept override {
		std::stringstream output;
		output << "setTimeout(function(){"<<std::endl;
		output << code->indented() << std::endl;
		output << "}, "<<int(1000.0f*time)<<");"<<std::endl;
		return output.str();
	}
};	

}
}
