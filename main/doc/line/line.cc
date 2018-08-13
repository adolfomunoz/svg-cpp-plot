#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg.add(Line(0,0,1,1));
	
	std::ofstream f("output.svg");
	f<<svg.automatic_viewBox();
}