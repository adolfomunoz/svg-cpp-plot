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
	auto g = _2d::group();
	g.add(_2d::line({0,-1},{0,1})).stroke(red).stroke_width(0.025).stroke_dasharray({0.1,0.1});
	g.add(_2d::rect({-3,-1},{3,1})).stroke(red).stroke_width(0.05).fill(none);
	g.add(_2d::curve_derivative(lissajous_curve(4,4,1,3), lissajous_derivative(4,4,1,3),0,2*M_PI)).stroke(green).stroke_width(0.1);
	g.add(_2d::points({{-2,0},{2,0}})).fill(black).stroke(black).stroke_width(0.2);
	g.add(_2d::text({0,1.7},"FOO")).font_size(0.9).text_anchor(text_anchor_middle).stroke_width(0.02).fill(red).stroke(black);
	svg.viewBox(BoundingBox(-5.1,-5.1,5.1,5.1));
	svg.add(_2d::group(_2d::identity)).add(g);
	svg.add(_2d::group(_2d::rotate(M_PI/2))).add(g);
	svg.add(_2d::group(_2d::scale(0.2,0.2))).add(g);
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg;
}
