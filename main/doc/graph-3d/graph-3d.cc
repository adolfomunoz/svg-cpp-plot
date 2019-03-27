#include "../../../svg-cpp-plot.h"
#include <fstream>

auto spiral(float r, float h, float p) {
	return [r,h,p] (float t) { 
		return std::tuple<float, float, float>
			(r*std::cos(t+p), h*t/(2*M_PI), r*std::sin(t+p)); 
	};
}

auto spiral_derivative(float r, float h, float p) {
	return [r,h,p] (float t) { 
		return std::tuple<float, float, float>
			(-r*std::sin(t+p), h/(2*M_PI), r*std::cos(t+p)); 
	};
}


using namespace svg_cpp_plot;
int main(int argc, char** argv) {
	SVG svg;
	Matrix m = matrix3::oblique;
	Group& group = svg.add(Group()); 
	Graph3D& graph = svg.add(Graph3D(m));
	group.add(plot_curve_derivative_3d(spiral(1,1,0), spiral_derivative(1,1,0),0,14*M_PI,m))
		.stroke(green).stroke_width(0.1);
	group.add(plot_curve_derivative_3d(spiral(0.5,2,4.0*M_PI/3.0), spiral_derivative(0.5,2,4.0*M_PI/3.0),0,8*M_PI,m))
		.stroke(red).stroke_width(0.1);
	group.add(plot_curve_derivative_3d(spiral(0.75,1.5,4.0*M_PI/3.0), spiral_derivative(0.75,1.5,4.0*M_PI/3.0),0,10*M_PI,m))
		.stroke(blue).stroke_width(0.1);
	graph.plot_curve_derivative_3d(spiral(1,1,0), spiral_derivative(1,1,0),0,14*M_PI)
		.stroke(green).stroke_width(0.1);
	graph.plot_curve_derivative_3d(spiral(0.5,2,4.0*M_PI/3.0), spiral_derivative(0.5,2,4.0*M_PI/3.0),0,8*M_PI)
		.stroke(red).stroke_width(0.1);
	graph.plot_curve_derivative_3d(spiral(0.75,1.5,4.0*M_PI/3.0), spiral_derivative(0.75,1.5,4.0*M_PI/3.0),0,10*M_PI)
		.stroke(blue).stroke_width(0.1);			

	group.set("transform","translate(-2,0)");
	graph.set("transform","translate(2,0)");
	
	std::ofstream f(std::string(argv[0])+".svg");
	f<<svg.automatic_viewBox(); //BoundingBox does not understand transforms
}
