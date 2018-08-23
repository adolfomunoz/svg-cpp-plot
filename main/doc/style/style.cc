#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg; Style style;
	svg.add(Circle(0,0,0.95)).class_("red");
	svg.add(Circle(0,2,0.95)).class_("yellow");
	svg.add(Circle(0,4,0.95)).class_("green");
	style.add("circle").fill("grey").stroke("black").stroke_width(0.04);
	style.add(".red:hover").fill("red");
	style.add(".yellow:hover").fill("yellow");
	style.add(".green:hover").fill("green");
	svg.add(style);
	
	std::ofstream f("output.svg");
	f<<svg.automatic_viewBox();
}
