#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg
	   .add(Polygon({{0,1},{0.5,0},{1,1}}).fill("red"))
	   .add(Polyline({{0,1},{0.5,0},{1,1}}).stroke("black").stroke_width(0.1));
	std::ofstream f("output.svg");
	f<<svg.automatic_viewBox();
}
