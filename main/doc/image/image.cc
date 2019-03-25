#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg.add(Image("../svg-tests/cat.jpg")).rect(0,0,1920,1080);
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
}
