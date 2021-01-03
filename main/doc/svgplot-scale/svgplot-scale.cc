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
    

    { // Example 3 
        svg_cpp_plot::SVGPlot plt;
        plt.subplot(1,3,0).title("Linear").set_yscale(svg_cpp_plot::linear);
        plt.subplot(1,3,1).title("Log base 2").set_yscale(svg_cpp_plot::log).base(2);
        plt.subplot(1,3,2).title("Log base 10").set_yscale(svg_cpp_plot::log).base(10);
        for (int p = 0; p<3;++p) plt.subplot(1,3,p).bar({"A","B","C","D"},{1,10,100,1000});
        plt.savefig("../doc/svgplot/scale/example3.svg");
    }
    
    { // Example 4  
        std::mt19937 gen{1}; //Fixed seed
        std::lognormal_distribution<float> d{0,1};
        std::list<float> samples;
        for (int n=0;n<1000;++n) { samples.push_back(d(gen)); }
        svg_cpp_plot::SVGPlot plt;
        plt.subplot(1,3,0).title("Linear").set_yscale(svg_cpp_plot::linear);
        plt.subplot(1,3,1).title("Log base 2").set_yscale(svg_cpp_plot::log).base(2);
        plt.subplot(1,3,2).title("Log base 10").set_yscale(svg_cpp_plot::log).base(10);
        for (int p = 0; p<3;++p) plt.subplot(1,3,p).hist(samples).bins(30);
        plt.savefig("../doc/svgplot/scale/example4.svg");
    }
    
    
}
