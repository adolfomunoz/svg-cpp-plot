#include "../../../svg-cpp-plot.h"
#include <fstream>


int main(int argc, char** argv) {
	{ // Example 1
		svg_cpp_plot::SVGPlot plt;
		plt.imshow({{0.0,0.1,0.2},
					{0.3,0.4,0.5},
					{0.6,0.7,0.8}});
		plt.savefig("../doc/svgplot/imshow/example1.svg");
	}
}
