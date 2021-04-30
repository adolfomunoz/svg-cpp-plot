#include "../../../svg-cpp-plot.h"

int main() {
    svg_cpp_plot::SVGPlot plt;
    plt.bar({"Apple","Orange","Banana","Kiwifruit","Blueberry","Grapes"},{35,30,9,26,39,4})
         .color({"red","orange","yellow","green","cyan","purple"});
    plt.savefig("nicest-fruit.png");
}    
