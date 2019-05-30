#include "../../../svg-cpp-plot.h"
#include <fstream>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	
	Graph2D& g1 = svg.add(_2d::group(_2d::translate(-250,0))).add(Graph2D({200,200},BoundingBox(-1,-1,1,1)));
	//Any 2d shape can be added to the drawing area (.area) (maybe later we need to include scaled primitives as well)
	for (float r=0.2;r<1.5;r+=0.2) g1.area().add(_2d::circle({0,0},r)).fill(none).stroke_width(1).stroke(black);
	
	for (float a=0;a<M_PI;a+=(0.2*M_PI)) 
		g1.area().add(_2d::line({-2*std::sin(a),-2*std::cos(a)},{2*std::sin(a),2*std::cos(a)})).stroke_width(1).stroke(red).stroke_dasharray({2,2});
	
	g1.border().stroke(black).stroke_width(2);


	Graph2D& g2 = svg.add(_2d::group())
		.add(Graph2D({200,200},BoundingBox(-2*M_PI,-1,2*M_PI,1)));
	//Any 2d shape can be added to the graph itself (maybe later extend to any primitive).
	g2.add(_2d::rect({-10,-10},{210,210})).stroke(black).stroke_width(0.5).fill(none);
	//Besides that, you can add plots
	g2.area().add(_2d::function([] (float x) { return sin(x); },-10,10)).fill(none).stroke_width(1).stroke(red);
	g2.area().add(_2d::function([] (float x) { return 1.3*cos(x+0.3); },-10,10)).fill(none).stroke_width(1).stroke(yellow);
	g2.area().add(_2d::function_derivative([] (float x) { return 0.5f*x*cos(x); },[] (float x) { return 0.5f*cos(x) - 0.5f*x*sin(x); },-10,10)).fill(none).stroke_width(1).stroke(green);
	g2.area().add(_2d::function([] (float x) { return 0.2f*cos(11.0f*x); },-10,10,1000)).fill(none).stroke_width(1).stroke(black);
	g2.border().stroke(black).stroke_width(1);
	g2.axis().stroke(black).stroke_width(0.5);
	g2.yticks(2,5).stroke(black).stroke_width(1);
	g2.xlabels().fill(black);
	g2.ylabels().fill(black);
	
	//In this third graph we will be adding points
	Graph2D& g3 = svg.add(_2d::group(_2d::translate(250,0)))
		.add(Graph2D({200,200},BoundingBox(-10,-10,10,10)));
	//We add common-colored points
	auto& ps = g3.area().add(_2d::points()).stroke_width(2).stroke(black);
	for (float x = -10.0; x<=10.0; x+=0.5) {
		ps.add_point(x,-0.5*x);
	}
	//We add points with different color
	for (float a=0;a<2*M_PI;a+=(0.1*M_PI)) g3.area().add(_2d::points({{5*std::cos(a),5*std::sin(a)}})).stroke_width(3).stroke(hsv(a,0.8,0.8));
	
	
	g3.border().stroke(black).stroke_width(1);
	g3.axis().stroke(black).stroke_width(0.5);
	g3.xlabels().fill(black);
	g3.ylabels().fill(black);
	
	//In this next three graphs we plot 2d functions and images
	svg.add(_2d::group(_2d::translate(-250,250)))
	   .add(Graph2D({200,200},BoundingBox(0,0,1,1)))
	   .area().add(_2d::function_image([] (float x, float y) { return std::tuple(x,y,(1.0f-x-y)); },{0,0},{1,1}));
	svg.add(_2d::group(_2d::translate(0,250)))
	   .add(Graph2D({200,200},BoundingBox(-1,-1,1,1)))
	   .area().add(_2d::function_2d([] (float x, float y) { return 1.0-sqrt(x*x+y*y);},{-1,-1},{1,1}));
	svg.add(_2d::group(_2d::translate(250,250)))
	   .add(Graph2D({200,200},BoundingBox(-4*M_PI,-4*M_PI,4*M_PI,4*M_PI)))
	   .area().add(_2d::function_2d([] (float x, float y) { return std::cos(x)*std::sin(y);},_2d::color_map_red_blue(-1,1),{-4*M_PI,-4*M_PI},{4*M_PI,4*M_PI},{500,500}));

	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox(BoundingBox(-275,-11,550,550)); //BoundingBox does not understand transforms
	
	
}
