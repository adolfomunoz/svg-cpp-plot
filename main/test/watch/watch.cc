#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <random>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	std::default_random_engine generator;
  	std::normal_distribution<float> distribution(0.0,2.0);
	svg.add(Line(0,-2,0,2)).stroke(black).stroke_width(0.05);
	svg.add(Line(-2,0,2,0)).stroke(black).stroke_width(0.05);
	svg.add(Script()).add(script::SetTimeout(script::WindowReload(),1));
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox();
	f.close();
	browse((std::string(argv[0])+".svg").c_str());

	while (true) {
		svg.add(Circle(distribution(generator), distribution(generator),0.1)).stroke(black).fill(red).stroke_width(0.01);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		f.open(std::string(argv[0])+".svg",std::ofstream::out | std::ofstream::trunc); 
		f<<svg.automatic_viewBox();
		f.close();
	}
}
