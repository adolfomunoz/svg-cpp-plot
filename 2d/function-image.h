#pragma once

#include "transform.h"
#include "../attributes/color.h"
#include "../3d/point-3d.h"
#include <cmath>

namespace svg_cpp_plot {
namespace _2d {
	
template<typename F>
auto function_image(const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const std::tuple<unsigned int,unsigned int> nsamples = {100,100}) {
	static_assert(is_3d_point_v<decltype(f(std::get<0>(xmin),std::get<1>(xmin)))>,
	       "Function f should return a color tuple (3d)");	

	auto s = group();
	s.stroke_width(0);
	unsigned int i, j;
	float dx = (std::get<0>(xmax)-std::get<0>(xmin))/float(std::get<0>(nsamples));
	float dy = (std::get<1>(xmax)-std::get<1>(xmin))/float(std::get<1>(nsamples));
	float x,  y;
	const float eps=0.05*dx;
	for (x=std::get<0>(xmin),i=0;i<std::get<0>(nsamples);x+=dx,++i)
		for (y=std::get<1>(xmin),j=0;j<std::get<1>(nsamples);y+=dy,++j)
			s.add(rect({x-eps,y-eps},{x+dx+eps,y+dy+eps})).fill(rgb(f(x+0.5*dx,y+0.5*dy))).stroke_width(0);
	return s;
}


}
}
