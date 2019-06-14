#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Mask& m = svg.add(Defs()).add(Mask());
	const float size = 1000;
	const float df = 0.05;
	for (float f = 0;f<1;f+=df) m.add(Rect(0,f*size,size,(f+0.5*df)*size)).fill(white);
	
	svg.add(Circle(0.5*size,0.5*size,0.5*size)).fill(red).mask(m);
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox(BoundingBox(0,0,size,size));
}
