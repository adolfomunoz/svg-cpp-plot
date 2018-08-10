#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg.add(Circle(0,0,1));
	svg.back()->attributes["stroke-width"]="0.2";
	svg.back()->attributes["stroke"]="black";
	svg.attributes["width"]="4";
	svg.attributes["height"]="4";
	svg.attributes["viewport"]="-2 -2 4 4";
	
	std::ofstream f("circle.svg");
	f<<svg;
}