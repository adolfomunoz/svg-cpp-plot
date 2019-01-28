#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Path heart(5,5);
	heart
	   .line_to(2,2)
	   .curve_to(1,1,4.8,1,5,2)
	   .curve_to(5.2,1,9,1,8,2)
	   .close()
	   .fill(red);
	svg
	   .add(heart);
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
}
