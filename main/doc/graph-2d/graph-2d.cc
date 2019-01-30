#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Graph2D& graph = svg.add(Graph2D({100,100},BoundingBox(-2*M_PI,-1,2*M_PI,1)));
	graph.id("graph-2d");
	graph.plot_function([] (float x) { return sin(x); }).stroke(yellow).stroke_width(1);
	graph.plot_function([] (float x) { return 1.3*cos(x); }).stroke(red).stroke_width(1);
	graph.plot_curve([] (float t) { return std::tuple<float,float>(M_PI*std::sin(t),std::cos(t)); },0.0,2*M_PI).stroke(blue).stroke_width(0.5).stroke_dasharray({1,2});
	graph.plot_points({{M_PI,0.2},{M_PI,-0.2},{-M_PI,0.2},{-M_PI,-0.2}},0.5).stroke_width(1).stroke(green).fill(yellow);
	graph.axis().stroke(black).stroke_width(0.5);
	graph.border().stroke(black).stroke_width(1);
	graph.ticks(5,5).stroke(black).stroke_width(0.5);
	graph.xlabels();
	graph.ylabels();
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox(); //BoundingBox does not understand transforms
}
