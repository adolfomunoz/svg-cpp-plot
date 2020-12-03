#pragma once

#include "../../2d/transform.h"
#include <array>

namespace svg_cpp_plot {
	
class Plottable : public Generator<_2d::Matrix>  {
    virtual std::array<float,4> axis() const = 0;

};
	
} // namespace svg_cpp_plot
