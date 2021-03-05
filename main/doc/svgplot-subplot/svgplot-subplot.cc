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
    
    { // Example 4
        auto r = [] (float x) { return 0.5f+0.5f*std::sin(x); };
        auto g = [] (float x) { return 0.5f+x*std::sin(x+M_PI/4.0); };
        auto b = [] (float x) { return 1.0 - x; };
        auto f = [&] (float x, float y) {
            float d = std::sqrt(x*x+y*y);
            return std::tuple(r(d),g(d),b(d));
        };        
    	svg_cpp_plot::SVGPlot plt;
        plt.subplot(2,1,0).subplot(1,3,0).figsize({200,200}).plot(svg_cpp_plot::linspace(0,1,25),r).color("red");
        plt.subplot(2,1,0).subplot(1,3,1).figsize({200,200}).plot(svg_cpp_plot::linspace(0,1,25),g).color("green");
        plt.subplot(2,1,0).subplot(1,3,2).figsize({200,200}).plot(svg_cpp_plot::linspace(0,1,25),b).color("blue");
        plt.subplot(2,1,1).figsize({600,600}).imshow(svg_cpp_plot::linspace(-1,1,50),svg_cpp_plot::linspace(-1,1,50),f);
		plt.savefig("../doc/svgplot/subplot/example4.svg");
    }
    
    { // Example 5
        std::mt19937 gen{1}; //Fixed seed
        std::normal_distribution<float> d{0,1};
        std::list<float> x, y;
        for (int n=0;n<1000;++n) { 
            y.push_back(d(gen)); x.push_back(0.8*y.back() + 0.5*d(gen));
        }
		svg_cpp_plot::SVGPlot plt; 
        plt.subplots_adjust().hspace(0).wspace(0).left(0.05).right(0.95).top(0.95).bottom(0.05);
        plt.subplot(2,2,2).figsize({300,300}).scatter(x,y).s(2).alpha(0.6);
        plt.subplot(2,2,0).figsize({300,150}).xticks({}).hist(x).bins(20).orientation(svg_cpp_plot::vertical);
        plt.subplot(2,2,3).figsize({150,300}).yticks({}).hist(y).bins(20).orientation(svg_cpp_plot::horizontal);
		plt.savefig("../doc/svgplot/subplot/example5.svg");
	}   
}
