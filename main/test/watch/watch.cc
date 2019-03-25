#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <random>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	std::string image_name = std::string(argv[0])+".image."+random_string()+".svg";
	std::string wrapper_name = std::string(argv[0])+".wrap."+random_string()+".svg";

	SVG internal_image;
	internal_image.add(Line(0,-2,0,2)).stroke(black).stroke_width(0.05);
	internal_image.add(Line(-2,0,2,0)).stroke(black).stroke_width(0.05);
	std::ofstream f(image_name);
	f<<internal_image.automatic_viewBox();
	f.close();

	SVG svg;
	Image& img = svg.add(Image(image_name)).rect(0,0,1280,720);
	svg.add(Script()).add(script::SetInterval(script::ForAll(std::string("#")+img.id(),script::Reload()),0.5));
//	svg.add(Script()).add(script::SetTimeout(script::WindowReload(),0.5));
	std::ofstream fw(wrapper_name);
	fw<<svg.automatic_viewBox();
	fw.close();
	browse(wrapper_name.c_str());

	std::default_random_engine generator;
  	std::normal_distribution<float> distribution(0.0,2.0);
	while (true) {
		internal_image.add(Circle(distribution(generator), distribution(generator),0.1)).stroke(black).fill(red).stroke_width(0.01);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		f.open(image_name,std::ofstream::out | std::ofstream::trunc); 
		f<<internal_image.automatic_viewBox();
		f.close();
	}
}
