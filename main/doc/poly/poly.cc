#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg.add(Polygon({{0,1},{0.5,0},{1,1}})).fill(red);
	svg.add(Polyline()).add_point(0,1).add_point(0.5,0).add_point(1,1).stroke(black).stroke_width(0.1);
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
}
