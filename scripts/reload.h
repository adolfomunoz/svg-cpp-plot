#pragma once

#include "../core/script.h"

namespace svg_cpp_plot {
namespace script {
class SelfReload : public CodeFunction<1> {
public:
	std::shared_ptr<Code> apply(const std::array<std::string,1>& params) const noexcept override {
		std::stringstream output;
		output << "var cloned = "<<params[0]<<".cloneNode(true);"<<std::endl;
		output << params[0]<<".parentNode.replaceChild(clone,"<<params[0]<<");"<<std::endl;
		return code_from_string(output.str());
	}
};	

}
}
