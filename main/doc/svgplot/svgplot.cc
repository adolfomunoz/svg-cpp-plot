#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	svg_cpp_plot::SVGPlot plt;
	plt.plot({1,2,3,4},{1,4,9,16});
	plt.savefig("output.svg");
}
