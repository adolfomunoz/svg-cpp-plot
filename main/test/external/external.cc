#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <random>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg_inner; svg_inner.add(Circle(0,0,1));
	std::string inner_name = std::string(argv[0])+"_inner.svg";
	std::ofstream fin(inner_name);
	fin<<svg_inner.automatic_viewBox();
	SVG svg; svg.add(Image(inner_name)); svg.viewBox(svg_inner.viewBox());
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg;
}
