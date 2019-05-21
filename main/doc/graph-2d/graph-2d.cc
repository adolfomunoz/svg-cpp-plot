#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Graph2D& graph = svg.add(Graph2D({100,100},BoundingBox(-2*M_PI,-1,2*M_PI,1)));
	//Any 2d shape can be added to the drawing area (.area) (maybe later we need to include any primitive 
	graph.area().add(_2d::rect({-1.5,-0.3},{1.5,0.3})).stroke(blue).stroke_width(2).stroke_dasharray({3,6}).fill(none);
	//Any 2d shape can be added to the graph itself (maybe later extend to any primitive).
	graph.add(_2d::rect({-10,-10},{110,110})).stroke(black).stroke_width(0.5).fill(none);
	//Besides that, you can add plots
	graph.area().add(plot::function([] (float x) { return sin(x); },-10,10)).fill(none).stroke_width(4).stroke(red);
	graph.area().add(plot::function([] (float x) { return 1.3*cos(x+0.3); },-10,10)).fill(none).stroke_width(4).stroke(yellow);
	graph.area().add(plot::function_derivative([] (float x) { return 0.5f*x*cos(x); },[] (float x) { return 0.5f*cos(x) - 0.5f*x*sin(x); },-10,10)).fill(none).stroke_width(4).stroke(green);
	graph.area().add(plot::function([] (float x) { return 0.2f*cos(11.0f*x); },-10,10,1000)).fill(none).stroke_width(4).stroke(black);
	auto& ps = graph.area().add(_2d::points());
	for (float x = -10.0; x<=10.0; x+=0.5) {
		ps.add_point(x,-0.5*x);
	}
	graph.axis().stroke(black).stroke_width(1);
	graph.ticks(2,2).stroke(black).stroke_width(2);
	graph.xlabels().fill(black);
	graph.ylabels().fill(black);
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox(BoundingBox(-11,-11,111,111)); //BoundingBox does not understand transforms
}
