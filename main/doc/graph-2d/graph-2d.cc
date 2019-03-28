#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Graph2D& graph = svg.add(Graph2D({100,100},BoundingBox(-2*M_PI,-1,2*M_PI,1)));
	graph.id("graph-2d");
//	graph.all_plots().stroke_width(1).hover().stroke_width(2);
	graph.area().add(plot_function([] (float x) { return sin(x); },-2*M_PI,2*M_PI)).stroke_width(4).stroke(yellow);
	graph.area().add(plot_function_area([] (float x) { return sin(x); },-2*M_PI,2*M_PI)).fill(yellow);
	graph.area().add(plot_function([] (float x) { return 1.3*cos(x); },-2*M_PI,2*M_PI)).stroke_width(4).stroke(red);
	graph.area().add(plot_function_area([] (float x) { return 1.3*cos(x); },-2*M_PI,2*M_PI)).fill(red);
	graph.area().add(plot_curve([] (float t) { return std::tuple<float,float>(M_PI*std::sin(t),std::cos(t)); },0.0,2*M_PI)).stroke(blue).stroke_width(2).stroke_dasharray({1,2});
	graph.axis().stroke(black).stroke_width(0.5);
	graph.border().stroke(black).stroke_width(1);
	graph.ticks(5,5).stroke(black).stroke_width(0.5);
	graph.xlabels();
	graph.ylabels();
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox(BoundingBox(-10,-3,100,110)); //BoundingBox does not understand transforms
}
