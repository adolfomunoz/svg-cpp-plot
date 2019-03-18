#pragma once

#include "element.h"
#include "attributes.h"
#include "presentation-attributes.h"
#include "text-presentation-attributes.h"
#include <memory>

namespace svg_cpp_plot {

class Code : public Object { };

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
