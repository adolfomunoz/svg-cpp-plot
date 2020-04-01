#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	{ // Example 1
		svg_cpp_plot::SVGPlot plt;
		plt.plot({1,2,3,4});
		plt.ylabel("some numbers");
		plt.savefig("../doc/example1.svg");
	}
	
	svg_cpp_plot::SVGPlot plt;
	plt.plot({1,-1,1,-1,1,-1});
	plt.plot({1,2,3,4},{0.1,.4,0.9,1.6});
	float f=-1.0;
	std::list<float> l; 
	for (int i=0; i<100;++i) l.push_back(f*=-1.005);
	plt.plot(svg_cpp_plot::arange(0,5,0.05),l,"g-");
	plt.savefig("output.svg");
}
