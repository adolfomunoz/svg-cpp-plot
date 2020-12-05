#include "../../../svg-cpp-plot.h"

int main(int argc, char** argv) {
    svg_cpp_plot::SVGPlot plt;
    plt.subplot(1,3,0).xlabel("How cool is this?").bar({"Meh","Fine","Awesome"},{0.1,1,6});
    plt.subplot(1,3,1).ylabel("Awesomeness").xlabel("Time").plot(svg_cpp_plot::arange(0.0,10.0,0.2),[] (float x) { return std::exp(x)/2000.0f; },"r--");
    plt.subplot(1,3,2).ylabel("C++ness").xlabel("Usefulness").imshow(svg_cpp_plot::arange(0.0,10.0,0.2),svg_cpp_plot::arange(0.0,10.0,0.2),[] (float x, float y) { return x+y; }).interpolation("bicubic").cmap("inferno");
    plt.savefig("../doc/svgplot/teaser.svg");
}
