#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	{ // Example 1
		svg_cpp_plot::SVGPlot plt;
        plt.bar({"G1","G2","G3","G4"},{1,2,3,4});
		plt.savefig("../doc/svgplot/bar/example1.svg");
	}
    
	{ // Example 1b
		svg_cpp_plot::SVGPlot plt;
        plt.bar(svg_cpp_plot::arange(4),{1,2,3,4});
        plt.xticks(svg_cpp_plot::arange(4),{"G1","G2","G3","G4"});
		plt.savefig("../doc/svgplot/bar/example1b.svg");
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
    
    { // Example 5
		svg_cpp_plot::SVGPlot plt;
        std::list<float> v1,v2;
        for (auto i : svg_cpp_plot::arange(50)) {
            v1.push_back(std::exp(-(float(i)-12.5f)*(float(i)-12.5f)/100.0f));
            v2.push_back(0.7*std::exp(-(float(i)-37.5f)*(float(i)-37.5f)/100.0f));            
        }
        plt.bar(svg_cpp_plot::arange(50),v1).width(1.0f).alpha(0.5f);
        plt.bar(svg_cpp_plot::arange(50),v2).width(1.0f).alpha(0.5f);
		plt.savefig("../doc/svgplot/bar/example5.svg");
	}
    
	{ // Example 6
		svg_cpp_plot::SVGPlot plt;
        std::vector<std::string> labels{"G1","G2","G3","G4"};
        std::vector<float> values{1,2,3,4};
        plt.subplot(1,3,0).bar(labels,values).color("#FF0000");
        plt.subplot(1,3,1).bar(labels,values).color({"r","b","y","g"});
        plt.subplot(1,3,2).bar(labels,values).color({"blue","magenta"});
//        plt.subplot(1,4,3).bar(labels,values).color({svg_cpp_plot::hsv(0,1,1),svg_cpp_plot::hsv(0.5*M_PI,1,1),svg_cpp_plot::hsv(M_PI,1,1),svg_cpp_plot::hsv(1.5*M_PI,1,1)});
		plt.savefig("../doc/svgplot/bar/example6.svg");
	} 
    
	{ // Example 10
		svg_cpp_plot::SVGPlot plt;
        float height = 0.3;
        std::vector<float> a{1,2,3,2,1};
        std::vector<float> b{2,1,2,1,2};
        plt.subplot(2,3,0).barh(svg_cpp_plot::arange(5)-0.5*height,a).height(height);
        plt.subplot(2,3,0).barh(svg_cpp_plot::arange(5)+0.5*height,b).height(height);
        plt.subplot(2,3,0).yticks(svg_cpp_plot::arange(5));
        plt.subplot(2,3,1).barh(svg_cpp_plot::arange(5),a);
        plt.subplot(2,3,1).barh(svg_cpp_plot::arange(5),b).left(a);
        plt.subplot(2,3,1).yticks(svg_cpp_plot::arange(5));
        std::list<float> v1,v2;
        for (auto i : svg_cpp_plot::arange(50)) {
            v1.push_back(std::exp(-(float(i)-12.5f)*(float(i)-12.5f)/100.0f));
            v2.push_back(0.7*std::exp(-(float(i)-37.5f)*(float(i)-37.5f)/100.0f));            
        }
        plt.subplot(2,3,2).barh(svg_cpp_plot::arange(50),v1).height(1.0f).alpha(0.5f);
        plt.subplot(2,3,2).barh(svg_cpp_plot::arange(50),v2).height(1.0f).alpha(0.5f);

        std::vector<std::string> labels{"G1","G2","G3","G4"};
        std::vector<float> values{1,2,3,4};
        plt.subplot(2,3,3).barh(labels,values).color("#FF0000");
        plt.subplot(2,3,4).barh(labels,values).color({"r","b","y","g"});
        plt.subplot(2,3,5).barh(labels,values).color({"blue","magenta"});
		plt.savefig("../doc/svgplot/bar/example10.svg");
	} 
}
