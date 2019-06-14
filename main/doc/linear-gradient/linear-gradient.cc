#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	LinearGradient& gradient = svg.add(Defs()).add(LinearGradient());
	gradient.add(Stop(0)).stop_color(red);
	gradient.add(Stop(100)).stop_color(blue);
	svg.add(Rect(0,0,3,2)).fill(gradient);
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
}
