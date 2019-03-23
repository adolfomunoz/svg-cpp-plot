#pragma once

#include "script.h"

namespace svg_cpp_plot {
namespace script {
class WindowReload : public Code {
public:
	std::string to_string() const noexcept override {
		return "window.location.reload(1)";
	}
};	

}
}
