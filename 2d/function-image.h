#pragma once

#include "transform.h"
#include "primitive.h"
#include "../primitives/linear-gradient.h"
#include "../primitives/image.h"
#include "../attributes/color.h"
#include "../3d/point-3d.h"
#include <cmath>
#include "../utils/png.h"
#include "../utils/base64.h"

namespace svg_cpp_plot {
namespace _2d {

namespace image_strategy {
	
class smooth_gradients_and_masks {
	std::tuple<unsigned int, unsigned int> nsamples;
	float border_expansion;
public:
	smooth_gradients_and_masks(const std::tuple<unsigned int,unsigned int>& nsamples = {100,100}, float border_expansion = 0.05) : nsamples(nsamples), border_expansion(border_expansion) {}
	template<typename F>
	void fill_image(_2d::Group& s, const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax) const {
		auto& gradients = s.add(defs());
		auto& image = s.add(group());
		image.stroke_width(0);
		unsigned int i, j;
		float dx = (std::get<0>(xmax)-std::get<0>(xmin))/float(std::get<0>(nsamples));
		float dxp = 100.0f/float(std::get<0>(nsamples));
		float dy = (std::get<1>(xmax)-std::get<1>(xmin))/float(std::get<1>(nsamples));
		float x, xp, y;
		const float eps = border_expansion*dx;
		//We first do the case for j=0
		{
			y=std::get<1>(xmin);
			LinearGradient& g = gradients.add(primitive(LinearGradient())).get().horizontal();
			for (x=std::get<0>(xmin),xp=0.0f,i=0;i<std::get<0>(nsamples);x+=dx,xp+=dxp,++i)
				g.add(Stop(xp)).stop_color(rgb(f(x,y)));
			g.add(Stop(100)).stop_color(rgb(f(std::get<0>(xmax),y)));
			auto& first_maskgradient =gradients.add(primitive(LinearGradient())).get().vertical();
			first_maskgradient.add(Stop(0)).stop_color(white);
			first_maskgradient.add(Stop(100)).stop_color(black);
			auto& verticalmask = gradients.add(mask());
			verticalmask.add(rect({std::get<0>(xmin),y-eps},{std::get<0>(xmax),y+dy+eps})).fill(first_maskgradient);
			image.add(rect({std::get<0>(xmin),y-eps},{std::get<0>(xmax),y+dy+eps})).stroke_width(0).fill(g).mask(verticalmask);

		}

		auto& maskgradient = gradients.add(primitive(LinearGradient())).get().vertical();
		maskgradient.add(Stop(50)).stop_color(white);
		maskgradient.add(Stop(100)).stop_color(black);

		for (y=std::get<1>(xmin)+dy,j=1;j<std::get<1>(nsamples);y+=dy,++j) {
			LinearGradient& g = gradients.add(primitive(LinearGradient())).get().horizontal();
			for (x=std::get<0>(xmin),xp=0.0f,i=0;i<std::get<0>(nsamples);x+=dx,xp+=dxp,++i)
				g.add(Stop(xp)).stop_color(rgb(f(x,y)));
			g.add(Stop(100)).stop_color(rgb(f(std::get<0>(xmax),y)));
			auto& verticalmask = gradients.add(mask());
			verticalmask.add(rect({std::get<0>(xmin),y-dy-eps},{std::get<0>(xmax),y+dy+eps})).fill(maskgradient);
			image.add(rect({std::get<0>(xmin),y-dy-eps},{std::get<0>(xmax),y+dy+eps})).stroke_width(0).fill(g).mask(verticalmask);
		}
		
		//We do the last case
		{
			float y=std::get<1>(xmax);
			LinearGradient& g = gradients.add(primitive(LinearGradient())).get().horizontal();
			for (x=std::get<0>(xmin),xp=0.0f,i=0;i<std::get<0>(nsamples);x+=dx,xp+=dxp,++i)
				g.add(Stop(xp)).stop_color(rgb(f(x,y)));
			g.add(Stop(100)).stop_color(rgb(f(std::get<0>(xmax),y)));
			image.add(rect({std::get<0>(xmin),y-dy-eps},{std::get<0>(xmax),y+eps})).stroke_width(0).fill(g);

		}
	}
};
	
class sharp_pixels {
	std::tuple<unsigned int, unsigned int> nsamples;
	float border_expansion;
public:
	sharp_pixels(const std::tuple<unsigned int,unsigned int>& nsamples = {100,100}, float border_expansion = 0.05) : nsamples(nsamples), border_expansion(border_expansion) {}
	template<typename F>
	void fill_image(_2d::Group& s, const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax) const {
		auto& image = s;
		image.stroke_width(0);
		unsigned int i, j;
		float dx = (std::get<0>(xmax)-std::get<0>(xmin))/float(std::get<0>(nsamples));
		float dy = (std::get<1>(xmax)-std::get<1>(xmin))/float(std::get<1>(nsamples));
		float x, y;
		const float eps = border_expansion*dx;

		for (y=std::get<1>(xmin),j=0;j<std::get<1>(nsamples);y+=dy,++j) 
			for (x=std::get<0>(xmin),i=0;i<std::get<0>(nsamples);x+=dx,++i)
				image.add(rect({x-eps,y-eps},{x+dx+eps,y+dy+eps})).stroke_width(0).fill(rgb(f(x+0.5*dx,y+0.5*dy)));
		}
};	

#ifdef USE_PNG
class embedded_png {
	std::tuple<unsigned int, unsigned int> nsamples;
public:
	embedded_png(const std::tuple<unsigned int,unsigned int>& _nsamples = {100,100}) : nsamples(_nsamples) {
		if (std::get<0>(nsamples)<1) std::get<0>(nsamples)=1;
		if (std::get<1>(nsamples)<1) std::get<1>(nsamples)=1;
	}
	
	template<typename F>
	void fill_image(_2d::Group& s, const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax) const {
//		std::stringstream ss;
//		ss << "external_" << std::setw(20) << std::setfill('0') << ids++ <<".png";
//		Image image(ss.str()); image.rect(xmin,xmax);
//		s.add(_2d::primitive(image));
		PNG png(std::get<0>(nsamples),std::get<1>(nsamples));
		unsigned int i, j;
		float dx = (std::get<0>(xmax)-std::get<0>(xmin))/float(std::get<0>(nsamples));
		float dy = (std::get<1>(xmax)-std::get<1>(xmin))/float(std::get<1>(nsamples));
		float x, y;

		for (y=std::get<1>(xmin),j=0;j<std::get<1>(nsamples);y+=dy,++j) 
			for (x=std::get<0>(xmin),i=0;i<std::get<0>(nsamples);x+=dx,++i)
				png.set_pixel(i,j,f(x+0.5*dx,y+0.5*dy));
			
		png.save("tmp.png");

		std::ifstream stream("tmp.png", std::ios::in | std::ios::binary);
		std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
		std::stringstream ss;
		ss << "data:image/png;base64,"<<base64_encode(contents.data(),contents.size());
		Image image(ss.str()); image.rect(xmin,xmax);
		s.add(_2d::primitive(image));
	}
};

#endif	

}

template<typename F, typename Strategy>
auto function_image(const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const Strategy& strategy) {
	static_assert(is_3d_point_v<decltype(f(std::get<0>(xmin),std::get<1>(xmin)))>,
	       "Function f should return a color tuple (3d)");	
	auto s = group();
	strategy.fill_image(s,f,xmin,xmax);
	return s;

}

	
template<typename F>
auto function_image(const F& f, const std::tuple<float,float>& xmin, const std::tuple<float,float>& xmax, const std::tuple<unsigned int,unsigned int> nsamples = {100,100}, float border = 0.05) {
	#ifndef USE_PNG
	return function_image(f,xmin,xmax,image_strategy::sharp_pixels(nsamples,border));
	//return function_image(f,xmin,xmax,image_strategy::smooth_gradients_and_masks(nsamples));
	#else
	return function_image(f,xmin,xmax,image_strategy::embedded_png(nsamples));	
	#endif
}


}
}
