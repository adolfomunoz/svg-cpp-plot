#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <random>

using namespace svg_cpp_plot;
int main(int argc, char** argv) {
    SlideShow slideshow;
    Group& slide1 = slideshow.add(Group());
    slide1.add(Rect()).x(0).y(0).width(Length(100,percentage)).height(Length(100,percentage)).fill(white);
    Group& slide2 = slideshow.add(Group());
    slide2.add(Rect()).x(0).y(0).width(Length(100,percentage)).height(Length(100,percentage)).fill(green);
//    slideshow.add(Polygon({{0,1},{0.5,0},{1,1}}));
	std::ofstream f(std::string(argv[0])+".svg");
	f<<slideshow;//.viewBox(BoundingBox(-1.5,-1.5,1.5,1.5));
}
