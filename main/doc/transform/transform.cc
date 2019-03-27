#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg; Style style;
	svg.add(Circle(0,0,1)).fill(red).stroke(black).stroke_width(0.04);
	svg.add(Circle(0,0,1)).fill(green).stroke(black).stroke_width(0.04).transform(translate(-1,-1));
	svg.add(Circle(0,0,1)).fill(yellow).stroke(black).stroke_width(0.04).transform(scale(2,0.5));
	svg.add(Circle(0,0,1)).fill(blue).stroke(black).stroke_width(0.04).transform({translate(1,1),rotate(45),scale(2,0.5)});
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox(BoundingBox(-3,-3,3,3));
}
