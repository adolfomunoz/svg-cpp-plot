#pragma once

#include "transform.h"
#include "primitive.h"
#include "../primitives/linear-gradient.h"
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
	auto& gradients = s.add(defs());
	auto& image = s.add(group());
	image.stroke_width(0);
	unsigned int i, j;
	float dx = (std::get<0>(xmax)-std::get<0>(xmin))/float(std::get<0>(nsamples));
	float dxp = 100.0f/float(std::get<0>(nsamples));
	float dy = (std::get<1>(xmax)-std::get<1>(xmin))/float(std::get<1>(nsamples));
	float x, xp, y;
	const float eps=0.05*dx;
	for (y=std::get<1>(xmin),j=0;j<std::get<1>(nsamples);y+=dy,++j) {
		LinearGradient& g = gradients.add(primitive(LinearGradient())).get().horizontal();
		for (x=std::get<0>(xmin),xp=0.0f,i=0;i<std::get<0>(nsamples);x+=dx,xp+=dxp,++i)
			g.add(Stop(xp)).stop_color(rgb(f(x,y+0.5*dy)));
		g.add(Stop(100)).stop_color(rgb(f(std::get<0>(xmax),y+0.5*dy)));
		image.add(rect({std::get<0>(xmin),y-eps},{std::get<0>(xmax),y+dy+eps})).stroke_width(0).fill(g);
	}
	
	return s;
}


}
}
