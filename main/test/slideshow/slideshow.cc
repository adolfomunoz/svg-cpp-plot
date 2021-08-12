#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <random>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
    SlideShow slideshow;
    slideshow.add(Circle(0,0,1));
    slideshow.add(Rect(0,0,1,1));
    slideshow.add(Polygon({{0,1},{0.5,0},{1,1}}));
	std::ofstream f(std::string(argv[0])+".svg");
	f<<slideshow.viewBox(BoundingBox(-1.5,-1.5,1.5,1.5));
}
