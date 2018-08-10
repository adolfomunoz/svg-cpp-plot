#pragma once 

#include "object.h"

namespace svg_cpp_plot {

class Circle : public Terminal {
public:
	Circle(float cx, float cy, float r) : Terminal("circle") 
	{
		attributes["cx"]=std::to_string(cx);
		attributes["cy"]=std::to_string(cy);
		attributes["r"] =std::to_string(r);		
	}
};

}