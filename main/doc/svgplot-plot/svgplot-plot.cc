#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	{ // Example 1
		svg_cpp_plot::SVGPlot plt;
		plt.plot({1,2,3,4});
		plt.ylabel("some numbers");
		plt.savefig("../doc/svgplot/plot/example1.svg");
	}

	{ // Example 2
		svg_cpp_plot::SVGPlot plt;
		plt.plot({1, 2, 3, 4}, {1, 4, 9, 16});
		plt.savefig("../doc/svgplot/plot/example2.svg");
	}
	
	{ // Example 3
		svg_cpp_plot::SVGPlot plt;
		float f=-1.0;
		std::list<float> l; 
		for (int i=0; i<100;++i) l.push_back(f*=-1.005);
		plt.plot(svg_cpp_plot::arange(0,5,0.05),l);
		plt.savefig("../doc/svgplot/plot/example3.svg");
	}
	
	{ // Example 4
		svg_cpp_plot::SVGPlot plt;
        auto x = svg_cpp_plot::linspace(0,5);
		plt.plot(x,[] (float x) { return x*std::sin(x);});
		plt.plot(x,sqrtf);
		plt.savefig("../doc/svgplot/plot/example4.svg");
	}
	
	{ // Example 5
		svg_cpp_plot::SVGPlot plt;
		plt.plot(svg_cpp_plot::arange(0,20,0.05),
				[] (float x) { return x*std::sin(x);},"g-");
		plt.plot(svg_cpp_plot::arange(0,20,0.05),
				[] (float x) { return x*std::sin(x+0.4*M_PI);},"r--");
		plt.plot(svg_cpp_plot::arange(0,20,0.05),
				[] (float x) { return x*std::sin(x+0.8*M_PI);},"b-.");
		plt.plot(svg_cpp_plot::arange(0,20,0.05),
				[] (float x) { return x*std::sin(x+1.2*M_PI);},"y:");
		plt.plot(svg_cpp_plot::arange(0,20,0.05),
				[] (float x) { return x*std::sin(x+1.6*M_PI);},"k");
		plt.plot(svg_cpp_plot::arange(0,20,0.25),
				[] (float x) { return 50+x*std::sin(x);},"go");
		plt.plot(svg_cpp_plot::arange(0,20,0.25),
				[] (float x) { return 50+x*std::sin(x+0.4*M_PI);},"r^");
		plt.plot(svg_cpp_plot::arange(0,20,0.25),
				[] (float x) { return 50+x*std::sin(x+0.8*M_PI);},"bs");
		plt.plot(svg_cpp_plot::arange(0,20,0.25),
				[] (float x) { return 50+x*std::sin(x+1.2*M_PI);},"y+");
		plt.plot(svg_cpp_plot::arange(0,20,0.25),
				[] (float x) { return 50+x*std::sin(x+1.6*M_PI);},"k.");

		plt.savefig("../doc/svgplot/plot/example5.svg");
	}
    
    { // Example 6
		svg_cpp_plot::SVGPlot plt;
		plt.plot(svg_cpp_plot::arange(0,7,0.05),[] (float x) { return std::sin(x); })
			.linestyle("-.").color( svg_cpp_plot::hsv(160,1,1)).linewidth(1);
		plt.plot(svg_cpp_plot::arange(0,7,0.5),[] (float x) { return std::sin(x); })
			.marker("s").color(svg_cpp_plot::rgb(0.2,0.8,0.2)).markersize(2);
 		plt.savefig("../doc/svgplot/plot/example6.svg");
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
