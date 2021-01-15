#include "../../../svg-cpp-plot.h"
#include <cmath>



int main(int argc, char** argv) {
	{ // Example 1
		svg_cpp_plot::SVGPlot plt; 
        for (int spx = 0; spx < 3; ++spx) for (int spy = 0; spy < 3; ++spy)
            plt.subplot(3,3,spy*3+spx).plot(
                svg_cpp_plot::linspace(0.0,2.0*M_PI,100),
                [&] (float x) { return std::sin(float(spx+1)*x)*std::sin(4*float(spy+1)*x); });
 		plt.savefig("../doc/svgplot/subplot/example1.svg");
	}
    
    { // Example 2
		svg_cpp_plot::SVGPlot plt; 
        auto x = svg_cpp_plot::linspace(0.0,1.0,100);
        for (int p = 0; p<4;++p) plt.subplot(2,2,p).figsize({200,200}).xticks({}).yticks({});
        plt.subplot(2,2,0).title("sqrt(x)").plot(x,sqrtf);
        plt.subplot(2,2,1).title("sqr(x)").plot(x,[] (float x) { return x*x; });
        plt.subplot(2,2,2).title("log(x+1)").plot(x,[] (float x) { return std::log(x+1); });
        plt.subplot(2,2,3).title("exp(x)").plot(x,expf);
		plt.savefig("../doc/svgplot/subplot/example2.svg");
	}
    
    { // Example 3
        std::mt19937 gen{1}; //Fixed seed
        std::normal_distribution<float> d{0,1};
        std::list<float> x, y;
        for (int n=0;n<1000;++n) { 
            y.push_back(d(gen)); x.push_back(0.8*y.back() + 0.5*d(gen));
        }
		svg_cpp_plot::SVGPlot plt; 
        plt.subplot(2,2,2).figsize({300,300}).scatter(x,y).s(2).alpha(0.6);
        plt.subplot(2,2,0).figsize({300,150}).xticks({}).hist(x).bins(20).orientation(svg_cpp_plot::vertical);
        plt.subplot(2,2,3).figsize({150,300}).yticks({}).hist(y).bins(20).orientation(svg_cpp_plot::horizontal);
		plt.savefig("../doc/svgplot/subplot/example3.svg");
	}   
}
