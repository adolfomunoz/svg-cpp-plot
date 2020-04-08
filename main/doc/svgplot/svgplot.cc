#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	{ // Example 1
		svg_cpp_plot::SVGPlot plt;
		plt.plot({1,2,3,4});
		plt.ylabel("some numbers");
		plt.savefig("../doc/example1.svg");
	}

	{ // Example 2
		svg_cpp_plot::SVGPlot plt;
		plt.plot({1, 2, 3, 4}, {1, 4, 9, 16});
		plt.savefig("../doc/example2.svg");
	}
	
	{ // Example 3
		svg_cpp_plot::SVGPlot plt;
		float f=-1.0;
		std::list<float> l; 
		for (int i=0; i<100;++i) l.push_back(f*=-1.005);
		plt.plot(svg_cpp_plot::arange(0,5,0.05),l);
		plt.savefig("../doc/example3.svg");
	}
	
	{ // Example 4
		svg_cpp_plot::SVGPlot plt;
		plt.plot(svg_cpp_plot::arange(0,5,0.05),
					[] (float x) { return x*std::sin(x);});
		plt.plot(svg_cpp_plot::arange(0,5,0.05),sqrtf);
		plt.savefig("../doc/example4.svg");
	}
	
	{ // Example 5
		svg_cpp_plot::SVGPlot plt;
		plt.plot(svg_cpp_plot::arange(0,20,0.05),
				[] (float x) { return x*std::sin(x);},"g-");
		plt.plot(svg_cpp_plot::arange(0,20,0.05),
				[] (float x) { return x*std::sin(x+0.5*M_PI);},"r--");
		plt.plot(svg_cpp_plot::arange(0,20,0.05),
				[] (float x) { return x*std::sin(x+M_PI);},"b-")
					.linewidth(0.5);
		plt.savefig("../doc/example5.svg");
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
