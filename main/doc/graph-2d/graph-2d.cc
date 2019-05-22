#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Graph2D& graph = svg.add(Graph2D({200,200},BoundingBox(-2*M_PI,-1,2*M_PI,1)));
	//Any 2d shape can be added to the drawing area (.area) (maybe later we need to include any primitive 
	graph.area().add(_2d::rect({-1.5,-0.3},{1.5,0.3})).stroke(blue).stroke_width(1).stroke_dasharray({1,2}).fill(none);
	//Any 2d shape can be added to the graph itself (maybe later extend to any primitive).
	graph.add(_2d::rect({-10,-10},{210,210})).stroke(black).stroke_width(0.5).fill(none);
	//Besides that, you can add plots
	graph.area().add(_2d::function([] (float x) { return sin(x); },-10,10)).fill(none).stroke_width(1).stroke(red);
	graph.area().add(_2d::function([] (float x) { return 1.3*cos(x+0.3); },-10,10)).fill(none).stroke_width(1).stroke(yellow);
	graph.area().add(_2d::function_derivative([] (float x) { return 0.5f*x*cos(x); },[] (float x) { return 0.5f*cos(x) - 0.5f*x*sin(x); },-10,10)).fill(none).stroke_width(1).stroke(green);
	graph.area().add(_2d::function([] (float x) { return 0.2f*cos(11.0f*x); },-10,10,1000)).fill(none).stroke_width(1).stroke(black);
	auto& ps = graph.area().add(_2d::points()).stroke_width(2).stroke(black);
	for (float x = -10.0; x<=10.0; x+=0.5) {
		ps.add_point(x,-0.5*x);
	}
	graph.border().stroke(black).stroke_width(1);
	graph.axis().stroke(black).stroke_width(0.5);
	graph.yticks(2,5).stroke(black).stroke_width(1);
	graph.xlabels().fill(black);
	graph.ylabels().fill(black);
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox(BoundingBox(-11,-11,211,211)); //BoundingBox does not understand transforms
}
