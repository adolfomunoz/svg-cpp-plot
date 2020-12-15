#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	{ // Example 1
        std::mt19937 gen{1}; //Fixed seed
        std::normal_distribution<> d{0,2};
        std::list<float> x,y;
        for (int n=0;n<50;++n) { x.push_back(d(gen)); y.push_back(d(gen)); }
        svg_cpp_plot::SVGPlot plt;
        plt.scatter(x,y);
        plt.savefig("../doc/svgplot/scatter/example1.svg");
	}
    
	{ // Example 2
        std::mt19937 gen{1}; //Fixed seed
        std::normal_distribution<> d{0,1};
        svg_cpp_plot::SVGPlot plt;
        for (int r = 0; r<3; ++r) {
            std::list<float> x,y;
            for (int s=0;s<25;++s) {
                x.push_back(-12.5+0.5*s+d(gen)); y.push_back(3*r+0.1*s+d(gen));
            }            
            plt.scatter(x,y);
        }            
        plt.savefig("../doc/svgplot/scatter/example2.svg");
	}
    
    { // Example 3
        std::mt19937 gen{1}; //Fixed seed
        std::fisher_f_distribution<float> d(5.0,5.0);
        std::list<float> x,y;
        for (int n=0;n<50;++n) { 
            x.push_back(d(gen)); y.push_back(d(gen)); 
        }
        svg_cpp_plot::SVGPlot plt;
        plt.scatter(x,y).c("r").alpha(0.5).s(2);
        plt.savefig("../doc/svgplot/scatter/example3.svg");
	}
    
    { // Example 4
        std::mt19937 gen{1}; //Fixed seed
        std::uniform_real_distribution<float> d(0,1);
        std::list<float> x,y;
        for (int i=0;i<10;++i) for (int j=0;j<10;++j) for (int s=0;s<2;++s) {
            x.push_back(i+d(gen)); y.push_back(j+d(gen)); 
        }
        svg_cpp_plot::SVGPlot plt; int p=0;
        for (std::string marker : {"o",".",",","v",">","^","<","s","+","P","x","X"})
            plt.subplot(4,3,p++).figsize({130,150}).title("Marker "+((marker!="<")?marker:"&lt;")).scatter(x,y).marker(marker);
        plt.savefig("../doc/svgplot/scatter/example4.svg");
	}
}
