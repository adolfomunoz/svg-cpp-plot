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
        std::mt19937 gen{1}; //Fixed seed
        std::lognormal_distribution<float> d{0,1};
        std::list<float> samples;
        for (int n=0;n<1000;++n) { samples.push_back(d(gen)); }
        svg_cpp_plot::SVGPlot plt;
        plt.subplot(1,3,0).title("Linear").set_yscale(svg_cpp_plot::linear);
        plt.subplot(1,3,1).title("Log base 2").set_yscale(svg_cpp_plot::log).base(2);
        plt.subplot(1,3,2).title("Log base 10").set_yscale(svg_cpp_plot::log).base(10);
        for (int p = 0; p<3;++p) plt.subplot(1,3,p).hist(samples).bins(30);
        plt.savefig("../doc/svgplot/scale/example3.svg");
    }
    
    { // Example 4  
        std::mt19937 gen{1}; //Fixed seed
        std::lognormal_distribution<float> d{0,1};
        std::list<float> p0, p1, n0, n1;
        for (int n=0;n<1000;++n) {  
            p0.push_back( d(gen)); p1.push_back( d(gen)); 
            n0.push_back(-d(gen)); n1.push_back(-d(gen));
        }
        svg_cpp_plot::SVGPlot plt;
        plt.subplot(1,3,0).figsize({200,200}).title("Linear").set_yscale(svg_cpp_plot::linear);
        plt.subplot(1,3,1).figsize({200,200}).title("Symlog base 2");
        plt.subplot(1,3,2).figsize({200,200}).title("Symlog base 10");
        plt.subplot(1,3,1).set_xscale(svg_cpp_plot::symlog).base(2);
        plt.subplot(1,3,2).set_xscale(svg_cpp_plot::symlog).base(10);
        plt.subplot(1,3,1).set_yscale(svg_cpp_plot::symlog).base(2);
        plt.subplot(1,3,2).set_yscale(svg_cpp_plot::symlog).base(10);
        for (int p = 0; p<3;++p) {
            plt.subplot(1,3,p).scatter(p0,p1);
            plt.subplot(1,3,p).scatter(p1,n0);
            plt.subplot(1,3,p).scatter(n0,n1);
            plt.subplot(1,3,p).scatter(n1,p0);
        }
        plt.savefig("../doc/svgplot/scale/example4.svg");
    }
    
    { // Example 5  
        std::mt19937 gen{1}; //Fixed seed
        std::lognormal_distribution<float> d{0,1};
        std::list<float> p0, p1, n0, n1;
        for (int n=0;n<1000;++n) {  
            p0.push_back( d(gen)); p1.push_back( d(gen)); 
            n0.push_back(-d(gen)); n1.push_back(-d(gen));
        }
        svg_cpp_plot::SVGPlot plt;
        plt.subplot(1,3,0).figsize({200,200}).title("Linear").set_yscale(svg_cpp_plot::linear);
        plt.subplot(1,3,1).figsize({200,200}).title("Symlog base 2");
        plt.subplot(1,3,2).figsize({200,200}).title("Symlog base 10");
        plt.subplot(1,3,1).set_xscale(svg_cpp_plot::symlog).base(2);
        plt.subplot(1,3,2).set_xscale(svg_cpp_plot::symlog).base(10);
        plt.subplot(1,3,1).set_yscale(svg_cpp_plot::symlog).base(2);
        plt.subplot(1,3,2).set_yscale(svg_cpp_plot::symlog).base(10);
        for (int p = 0; p<3;++p) {
            plt.subplot(1,3,p).scatter(p0,p1);
            plt.subplot(1,3,p).scatter(p1,n0);
            plt.subplot(1,3,p).scatter(n0,n1);
            plt.subplot(1,3,p).scatter(n1,p0);
        }
        plt.savefig("../doc/svgplot/scale/example4.svg");
    }
    
    
}
