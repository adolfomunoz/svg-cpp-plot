#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg.add(Circle(0,0,1));
	
	std::ofstream f("output.svg");
	f<<svg.automatic_viewBox();
}