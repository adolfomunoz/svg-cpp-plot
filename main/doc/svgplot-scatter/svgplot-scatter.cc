#include "../../../svg-cpp-plot.h"
#include <fstream>



int main(int argc, char** argv) {
	{ // Example 1
        std::mt19937 gen{1}; //Fixed seed
        std::normal_distribution<float> d{0,2};
        std::list<float> x,y;
        for (int n=0;n<50;++n) { x.push_back(d(gen)); y.push_back(d(gen)); }
        svg_cpp_plot::SVGPlot plt;
        plt.scatter(x,y);
        plt.figsize({200,200}).savefig("../doc/svgplot/scatter/example1.svg");
	}
    
	{ // Example 2
        std::mt19937 gen{1}; //Fixed seed
        std::normal_distribution<float> d{0,1};
        svg_cpp_plot::SVGPlot plt;
        for (int r = 0; r<3; ++r) {
            std::list<float> x,y;
            for (int s=0;s<25;++s) {
                x.push_back(-12.5+0.5*s+d(gen)); y.push_back(3*r+0.1*s+d(gen));
            }            
            plt.scatter(x,y);
        }            
        plt.figsize({200,200}).savefig("../doc/svgplot/scatter/example2.svg");
	}
    
    { // Example 3
        std::mt19937 gen{1}; //Fixed seed
        std::fisher_f_distribution<float> d(5.0,5.0);
        std::list<float> x,y;
        for (int n=0;n<50;++n) { 
            x.push_back(d(gen)); y.push_back(d(gen)); 
        }
        svg_cpp_plot::SVGPlot plt;
        plt.scatter(x,y).c("r").alpha(0.5).s(2).edgecolors("black").linewidths(0.5);
        plt.figsize({200,200}).savefig("../doc/svgplot/scatter/example3.svg");
	}
    
    { // Example 4
        svg_cpp_plot::SVGPlot plt; int p=0;
        for (std::string marker : {"o",".",",","v",">","^","<","s","+","P","x","X"}) {
            auto& sub = plt.subplot(2,6,p++);
            sub.set_title("'"+((marker!="<")?marker:"&lt;")+"'").fontsize(2);
            sub.axis({-1,1,-1,1}).xticks({}).yticks({}).figsize({5,8}).linewidth(0.01);
            sub.scatter({0},{0}).marker(marker);
         }
        plt.savefig("../doc/svgplot/scatter/example4.svg");
	}
    
    { // Example 5
        std::mt19937 gen{1}; //Fixed seed
        std::uniform_real_distribution<float> d(0,1);
        std::list<float> x,y, value;
        for (int i=0;i<10;++i) for (int j=0;j<10;++j) for (int s=0;s<3;++s) {
            x.push_back(i+d(gen)); y.push_back(j+d(gen)); value.push_back(sqrt((x.back()-5.0f)*(x.back()-5.0f)+(y.back()-5.0f)*(y.back()-5.0f)));
        }
        svg_cpp_plot::SVGPlot plt; int p = 0;
        for (auto cmap : {"grayscale","viridis","plasma"})
            plt.subplot(1,3,p++).figsize({200,200}).scatter(x,y).c(value).cmap(cmap);
        plt.savefig("../doc/svgplot/scatter/example5.svg");
    }
        
    { // Example 6
        svg_cpp_plot::SVGPlot plt;
        plt.scatter({0,0,0},{0,2,4}).c({{0,1,0.25},{1.0,0.75,0},{0.75,0.25,0.25}});
        plt.figsize({10,25}).axis({-2,2,-2,6}).xticks({}).yticks({}).linewidth(0);
        plt.savefig("../doc/svgplot/scatter/example6.svg");
    }
    
    { // Example 7
        svg_cpp_plot::SVGPlot plt;
        plt.scatter({0,0,0},{0,2,3.5}).s({4,3,2});
        plt.scatter({0},{3.4}).s(0.4).marker("v");
        plt.scatter({-0.3,0.3,-0.3,-0.1,0.1,0.3},{3.8,3.8,3.2,3.2,3.2,3.2}).s({0.4,0.4,0.3,0.2,0.2,0.3}).marker("+");
        plt.figsize({10,25}).axis({-2,2,-2,5}).xticks({}).yticks({}).linewidth(0);
        plt.savefig("../doc/svgplot/scatter/example7.svg");
    }

}
