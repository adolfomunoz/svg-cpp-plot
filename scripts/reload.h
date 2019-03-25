#pragma once

#include "script.h"

namespace svg_cpp_plot {
namespace script {
class Reload : public CodeFunction<1> {
public:
	std::shared_ptr<Code> apply(const std::array<std::string,1>& params) const noexcept override {
		std::stringstream output;
		output << "var cloned = "<<params[0]<<".cloneNode(true);"<<std::endl;
		output << "cloned.setAttribute(\"href\",cloned.getAttribute(\"data-href\")+\"?\"+ new Date().getTime());"<<std::endl;
		output << params[0]<<".parentNode.replaceChild(cloned,"<<params[0]<<");"<<std::endl;
		return code_from_string(output.str());
	}
};	

}
}
