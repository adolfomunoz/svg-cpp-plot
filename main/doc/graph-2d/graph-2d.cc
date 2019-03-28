#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Graph2D& graph = svg.add(Graph2D({100,100},BoundingBox(-2*M_PI,-1,2*M_PI,1)));
	//Anything (any shape) can be added to the drawing area (.area) 
	graph.area().add(plot_curve([] (float t) { return std::tuple<float,float>(M_PI*std::sin(t),std::cos(t)); },0.0,2*M_PI)).stroke(blue).stroke_width(3).stroke_dasharray({5,10});
	//Anything can be added to the graph itself (it is a group).
	graph.add(Rect(-10,-10,110,110)).stroke(black).stroke_width(0.5).fill(none);
	//Besides that, there are specialized methods for most things that take care of everything, including plotting functions, axis, automatic styling, cutting of borders...
	graph.plot_function([] (float x) { return sin(x); }).plot_function([] (float x) { return 1.3*cos(x); }).axis().ticks(5,5).xlabels().ylabels();
	//Styling can also be done (plots are all the plots)
	graph.graph_style().add_class("plot").hover().stroke_width(10);
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox(BoundingBox(-11,-11,111,111)); //BoundingBox does not understand transforms
}
