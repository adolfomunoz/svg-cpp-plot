#pragma once

#include "../../2d/transform.h"
#include <array>
#include "axis-scale.h"

namespace svg_cpp_plot {
	
class Plottable {
public:
    virtual std::shared_ptr<_2d::Element> scaled(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept = 0;
    virtual std::array<float,4> axis() const noexcept = 0;
};
	
} // namespace svg_cpp_plot
