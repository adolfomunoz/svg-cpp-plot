#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	svg_cpp_plot::SVGPlot plt;
	plt.plot({1,2,3,4},{1,4,9,16});
	plt.plot(svg_cpp_plot::arange(0,5,1),{1,4,9,16,25});
	plt.plot({1,3,5,7});
	plt.plot({1,2,3,4,5},{2,3,4,5,6},"r-");
	plt.savefig("output.svg");
}
