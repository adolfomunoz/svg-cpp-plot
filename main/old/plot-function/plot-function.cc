#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <cmath>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg.add(
		plot_function_derivative([] (float x) { return std::sin(x); }, [] (float x) { return std::cos(x); },0.0f,15.0f)
		   .stroke(green).stroke_width(0.1)
	       );
	svg.add(plot_function([] (float x) { return 4.0f*std::sin(2.0f*x+1.0f)/(x+1.0f); }, 0.0f,15.0f))
		   .stroke(red).stroke_width(0.1);
	svg.add(Line(0.0,0.0,15.0,0.0)).stroke(black).stroke_width(0.1);
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
}
