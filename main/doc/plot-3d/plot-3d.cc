#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <cmath>



auto spiral(float r, float h) {
	return [r,h] (float t) { 
		return std::tuple<float, float, float>
			(r*std::cos(t), h*t/(2*M_PI), r*std::sin(t)); 
	};
}

auto spiral_derivative(float r, float h) {
	return [r,h] (float t) { 
		return std::tuple<float, float, float>
			(-r*std::sin(t), h/(2*M_PI), r*std::cos(t)); 
	};
}



using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	
	svg.add(plot_curve_derivative_3d(spiral(1,1), spiral_derivative(1,1),0,12*M_PI,matrix3::isometric))
		   .stroke(green).stroke_width(0.1);
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
}
