#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg.add(Rect(0,0,3,2));
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
}
