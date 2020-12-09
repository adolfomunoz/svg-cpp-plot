#include "../../../svg-cpp-plot.h"
#include <random>

int main(int argc, char** argv) {
	{ // Example 1
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> d{5,2};
        std::list<float> samples;
        for (int n=0;n<1000;++n) { samples.push_back(d(gen)); }
        svg_cpp_plot::SVGPlot plt;
        plt.hist(samples);
        plt.savefig("../doc/svgplot/hist/example1.svg");
	}
    
	{ // Example 2
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> d{5,2};
        std::list<float> samples;
        for (int n=0;n<1000;++n) { samples.push_back(d(gen)); }
        svg_cpp_plot::SVGPlot plt;
        std::vector<std::size_t> nbins{4,10,40};
        for (std::size_t b = 0; b<nbins.size();++b)
            plt.subplot(1,nbins.size(),b).title(std::to_string(nbins[b])+" bins").hist(samples).bins(nbins[b]);
        plt.savefig("../doc/svgplot/hist/example2.svg");
	}
    
    { // Example 3
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> d{5,2};
        std::list<float> samples;
        for (int n=0;n<1000;++n) { samples.push_back(d(gen)); }
        svg_cpp_plot::SVGPlot plt;
        plt.subplot(2,2,0).title("Standard").hist(samples);
        plt.subplot(2,2,1).title("Cumulative").hist(samples).cumulative();
        plt.subplot(2,2,2).title("Density").hist(samples).density();
        plt.subplot(2,2,3).title("Density Cumulative.").hist(samples).density().cumulative();
        plt.savefig("../doc/svgplot/hist/example3.svg");
	}
    
	{ // Example 4
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> d{5,2};
        std::list<float> samples;
        for (int n=0;n<1000;++n) { samples.push_back(d(gen)); }
        svg_cpp_plot::SVGPlot plt;
        std::vector<std::string> orientations{"vertical","horizontal"};
        std::vector<std::string> histtypes{"bar","step"};
        for (std::size_t o = 0; o<orientations.size(); ++o) for (std::size_t t = 0; t<histtypes.size(); ++t)
            plt.subplot(orientations.size(),histtypes.size(),t+histtypes.size()*o).hist(samples)
                .orientation(orientations[o]).histtype(histtypes[t]);
        plt.savefig("../doc/svgplot/hist/example4.svg");
	}
    

}
