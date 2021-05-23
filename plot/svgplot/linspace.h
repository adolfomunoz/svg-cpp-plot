#pragma once

#include "arange.h"

namespace svg_cpp_plot {
auto linspace(float start, float stop, int nsamples = 50) {
    float step = (stop - start)/float(nsamples-1);
    return arange(start,stop+step,step);
}
}