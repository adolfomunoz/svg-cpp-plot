#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Graph2D& graph = svg.add(Graph2D({100,100},BoundingBox(-2*3.141592,-0.5,2*3.141592,0.5)));
	graph.id("graph-2d");
	graph.plot_function([] (float t) { return sin(t); }).stroke(yellow).stroke_width(1);
	graph.axis().stroke(black).stroke_width(1);
	std::ofstream f("output.svg");
	f<<svg.automatic_viewBox(); //BoundingBox does not understand transforms
}
