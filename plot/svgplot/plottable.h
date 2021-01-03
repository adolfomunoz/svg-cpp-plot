#pragma once

#include "../../2d/transform.h"
#include <array>
#include "axis-scale.h"

namespace svg_cpp_plot {

std::array<float,4> axis_join(const std::array<float,4>& a, const std::array<float,4>& b) {
    return std::array<float,4>{std::min(a[0],b[0]), std::max(a[1],b[1]), std::min(a[2],b[2]), std::max(a[3],b[3])};
}

std::array<float,4> axis_transform(const std::array<float,4>& a, const axis_scale::Base& xscale, const axis_scale::Base& yscale) {
    std::array<float,4> sol = { xscale.transform(a[0]), xscale.transform(a[1]), yscale.transform(a[2]), yscale.transform(a[3]) };
    if (!xscale.is_valid(a[0])) sol[0] = sol[1];
    if (!xscale.is_valid(a[1])) sol[1] = sol[0];
    if (!yscale.is_valid(a[2])) sol[2] = sol[3];
    if (!yscale.is_valid(a[3])) sol[3] = sol[2];
    return sol;
}

std::array<float,4> axis_expand(const std::array<float,4>& a, float r = 1.0f/32.0f) {
    float dx = r*(a[1]-a[0]); float dy = r*(a[3]-a[2]);
    return std::array<float,4>{a[0]-dx,a[1]+dx,a[2]-dy,a[3]+dy};
}

class Plottable {
public:
    virtual std::shared_ptr<_2d::Element> scaled(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept = 0;
    virtual std::array<float,4> axis() const noexcept = 0;
    virtual std::array<float,4> scaled_axis(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept {
        auto ax = axis();
        return std::array<float,4>{xscale.transform(ax[0]),xscale.transform(ax[1]),
            yscale.transform(ax[2]),yscale.transform(ax[3])};        
    }
};
	
} // namespace svg_cpp_plot
