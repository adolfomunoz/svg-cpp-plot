#pragma once

#include "script.h"

namespace svg_cpp_plot {
namespace script {
class SetInterval : public Code {
	std::shared_ptr<Code> code;
	float time; //In seconds;
public:
	SetInterval(std::shared_ptr<Code> code, float time) : code(code), time(time) { }
	template<typename C, typename = std::enable_if_t<std::is_base_of_v<Code,C>> >
	SetInterval(const C& code, float time) : SetInterval(std::make_shared<C>(code),time) { }

	std::string to_string() const noexcept override {
		std::stringstream output;
		output << "setInterval(function(){"<<std::endl;
		output << code->indented() << std::endl;
		output << "}, "<<int(1000.0f*time)<<");"<<std::endl;
		return output.str();
	}
};	

}
}
