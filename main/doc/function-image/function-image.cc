#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	
	const auto f = [] (float x, float y) {
		return std::tuple(x,y,(1.0f-x-y));
	};
	const std::tuple<float, float> xmin(0,0);
	const std::tuple<float, float> xmax(1,1);
	const std::tuple<float, float> size(450,450);
	const std::tuple<float, float> space(50,50);
	std::vector<float> resolutions = {4,20,100,500};
	
	for (std::size_t i = 0;i<resolutions.size();++i) {
		svg.add(_2d::group(_2d::translate(i*(std::get<0>(size)+std::get<0>(space)),0)))
			.add(Graph2D(size,BoundingBox(xmin,xmax)))
			.area().add(_2d::function_image(f,xmin,xmax,
				_2d::image_strategy::sharp_pixels({resolutions[i],resolutions[i]})));
		svg.add(_2d::group(_2d::translate(i*(std::get<0>(size)+std::get<0>(space)),std::get<1>(size)+std::get<1>(space))))
			.add(Graph2D(size,BoundingBox(xmin,xmax)))
			.area().add(_2d::function_image(f,xmin,xmax,
				_2d::image_strategy::smooth_gradients_and_masks({resolutions[i],resolutions[i]})));

	}
	
	svg.viewBox(BoundingBox(0,0,resolutions.size()*(std::get<0>(size)+std::get<0>(space)),2*(std::get<1>(size)+std::get<1>(space))));

	
	std::ofstream out(std::string(argv[0])+".svg");
	out<<svg;
	
	
}
