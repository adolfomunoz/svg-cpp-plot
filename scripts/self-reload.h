#pragma once

#include "../core/script.h"

namespace svg_cpp_plot {
namespace script {
class SelfReload : public Code {
public:
	std::string to_string() const noexcept override {
		return "window.location.reload(1)";
	}
};	

}
}
