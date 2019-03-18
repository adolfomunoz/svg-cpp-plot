#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	svg.add(Circle(0,0,1));
	svg.add(Script()).add(script::SelfReload(5));
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
	browse((std::string(argv[0])+".svg").c_str());
}
