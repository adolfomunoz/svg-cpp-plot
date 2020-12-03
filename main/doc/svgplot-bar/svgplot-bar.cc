#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	{ // Example 1
		svg_cpp_plot::SVGPlot plt;
        plt.bar(svg_cpp_plot::arange(4),{1,2,3,4});
        plt.xticks(svg_cpp_plot::arange(4),{"G1","G2","G3","G4"});
		plt.savefig("../doc/svgplot/bar/example1.svg");
	}
    
    { // Example 2
		svg_cpp_plot::SVGPlot plt;
        std::list<float> v;
        for (auto i : svg_cpp_plot::arange(25))
            v.push_back(std::exp(-(float(i)-12.5f)*(float(i)-12.5f)/100.0f));
        plt.bar(svg_cpp_plot::arange(25),v).width(1.0f);
		plt.savefig("../doc/svgplot/bar/example2.svg");
	}
    
    { // Example 3
		svg_cpp_plot::SVGPlot plt;
        float width = 0.3;
        std::vector<float> a{1,2,3,2,1};
        std::vector<float> b{2,1,2,1,2};
        plt.bar(svg_cpp_plot::arange(5)-0.5*width,a).width(width);
        plt.bar(svg_cpp_plot::arange(5)+0.5*width,b).width(width);
        plt.xticks(svg_cpp_plot::arange(5));
		plt.savefig("../doc/svgplot/bar/example3.svg");
	}
    
    { // Example 4
		svg_cpp_plot::SVGPlot plt;
        std::vector<float> a{1,2,3,2,1};
        std::vector<float> b{2,1,2,1,2};
        plt.bar(svg_cpp_plot::arange(5),a);
        plt.bar(svg_cpp_plot::arange(5),b).bottom(a);
        plt.xticks(svg_cpp_plot::arange(5));
		plt.savefig("../doc/svgplot/bar/example4.svg");
	}
}
