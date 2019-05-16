#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <cmath>

auto lissajous_curve(float a, float b, float k_a, float k_b) {
	return [a,b,k_a,k_b] (float t) { return std::tuple(a*std::cos(k_a*t), b*std::sin(k_b*t)); };
}

auto lissajous_derivative(float a, float b, float k_a, float k_b) {
	return [a,b,k_a,k_b] (float t) { 
		return std::tuple(-a*k_a*std::sin(k_a*t), b*k_b*std::cos(k_b*t)); };
}


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	auto group = svg.add(GroupGenerator<_2d::Matrix>(_2d::identity));
	group.add(_2d::curve_derivative(lissajous_curve(4,4,1,3), lissajous_derivative(4,4,1,3),0,2*M_PI)).stroke(green).stroke_width(0.1);
	std::ofstream f(std::string(argv[0])+".svg");
	svg.viewBox(BoundingBox(-0.1,-0.1,1.1,1.1));
	f<<svg;
}
