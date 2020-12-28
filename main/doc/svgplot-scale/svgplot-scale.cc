#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	{ // Example 1
		svg_cpp_plot::SVGPlot plt; int p = 0;
        auto x = svg_cpp_plot::arange(0.1,1.0,0.01);
        auto f = [] (float x) { return x; };
        for (auto xscale : {"linear","log","symlog"}) for (auto yscale : {"linear","log","symlog"})
            plt.subplot(3,3,p++).xlabel(xscale).ylabel(yscale).figsize({300,300}).xscale(xscale).yscale(yscale).plot(x,f);
		plt.savefig("../doc/svgplot/scale/example1.svg");
	}
    
    { // Example 2
		svg_cpp_plot::SVGPlot plt; int p = 0;
        auto x = svg_cpp_plot::arange(-1.0,1.0,0.01);
        auto f = [] (float x) { return x; };
        for (auto xscale : {"linear","log","symlog"}) for (auto yscale : {"linear","log","symlog"})
            plt.subplot(3,3,p++).xlabel(xscale).ylabel(yscale).figsize({300,300}).xscale(xscale).yscale(yscale).plot(x,f);
		plt.savefig("../doc/svgplot/scale/example2.svg");
	}
}
