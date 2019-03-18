#pragma once

#include "../core/script.h"
#include <sstream>

namespace svg_cpp_plot {
namespace script {
class SelfReload : public Code {
	float time; //In seconds;
public:
	SelfReload(float time) : time(time) { }
	std::string to_string() const noexcept override {
		std::stringstream code;
		code << "setTimeout(function(){"<<std::endl;
		code << "     window.location.reload(1);"<<std::endl;
		code << "}, "<<int(1000.0f*time)<<");"<<std::endl;
		return code.str();
	}
};	

}
}
