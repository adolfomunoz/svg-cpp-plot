#include "../../../svg-cpp-plot.h"
#include <fstream>


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg; 
	Group& g = svg.add(Group()); 
	GroupZOrdered& zg = svg.add(GroupZOrdered());
	Circle c1(0,0,1); c1.fill(red);
	Circle c2(0.5,0.5,1.2); c2.fill(yellow);
	Circle c3(1.1,1.1,1.4); c3.fill(green);
	g.transform(translate(-2));
	zg.transform(translate(2));
	g.add(c1); zg.add(c1,-1);
	g.add(c2); zg.add(c2, 0);
	g.add(c3); zg.add(c3, 1);
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.viewBox({-1.5,-1.5,3,3}); //BoundingBox does not understand transforms
}
