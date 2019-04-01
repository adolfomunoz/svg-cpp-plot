#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Graph2D& graph = svg.add(Graph2D({100,100},BoundingBox(-2*M_PI,-1,2*M_PI,1)));
	//Anything (any shape) can be added to the drawing area (.area) 
	graph.area().add(Rect(-1.5,-0.3,1.5,0.3)).stroke(blue).stroke_width(2).stroke_dasharray({3,6}).fill(none);
	//Anything can be added to the graph itself (it is a group).
	graph.add(Rect(-10,-10,110,110)).stroke(black).stroke_width(0.5).fill(none);
	//Besides that, there are specialized methods for most things that take care of everything, including plotting functions, axis, automatic styling, cutting of borders...
	graph.add_plot(plot_function([] (float x) { return sin(x); },-10,10))
	     .add_plot(plot_function([] (float x) { return 1.3*cos(x+0.3); },-10,10))
	     .add_plot(plot_function_derivative([] (float x) { return 0.5f*x*cos(x); },[] (float x) { return 0.5f*cos(x) - 0.5f*x*sin(x); },-10,10))
	     .add_plot(plot_function([] (float x) { return 0.2f*cos(11.0f*x); },-10,10,1000))
	     .axis().
	     ticks(5,5).
	     xlabels().
	     ylabels();
	
	//Styling can also be done (plot represents all the plots)
	graph.graph_style().add_class("plot").hover().stroke_width(10);
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox(BoundingBox(-11,-11,111,111)); //BoundingBox does not understand transforms

	graph.graph_style(graph_style::GrayScale());
	std::ofstream fg(std::string(argv[0])+"-bw.svg");
	fg<<svg;
}
