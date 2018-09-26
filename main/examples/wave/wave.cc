#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <cstring>

using namespace svg_cpp_plot;

int main(int argc, char** argv) {
	float lambda = 1.0;
	float length = 6.0;
	float power_x  = 1.0; float phase_x = 0.0;
	float power_y  = 1.0; float phase_y = M_PI/2.0;
	const char* filename = "output.svg";
	
	for (int i=0;i<argc-1;++i) {
		if (strcmp(argv[i],"-lambda")==0) lambda = atof(argv[++i]);
		else if (strcmp(argv[i],"-length")==0) length = atof(argv[++i]);
		else if (strcmp(argv[i],"-power-x")==0) power_x = atof(argv[++i]);
		else if (strcmp(argv[i],"-power-y")==0) power_y = atof(argv[++i]);
		else if (strcmp(argv[i],"-phase-x")==0) phase_x = (M_PI/180.0f)*atof(argv[++i]);
		else if (strcmp(argv[i],"-phase-y")==0) phase_y = (M_PI/180.0f)*atof(argv[++i]);
		else if (strcmp(argv[i],"-filename")==0) filename = argv[++i];
	}
	
	SVG svg;
	Graph3D& graph = svg.add(Graph3D(matrix::oblique*matrix::zyx));

	auto wave = [&] (float t) {	return std::tuple(power_x*sin(2*M_PI*t/lambda + phase_x),
						power_y*sin(2*M_PI*t/lambda + phase_y),t); };
	
	graph.line(std::tuple(0.0f,0.0f,0.0f),std::tuple(0.0f,0.0f,length*1.1f))
		.stroke(black).stroke_width(0.04*power_x);
	graph.plot_curve_3d(project_to_x(wave,1.1f), 0.0,length, 4*std::max(1,int(length/lambda)))
		.stroke(rgb(0.5,0.8,0)).stroke_width(0.02*power_x);
	graph.plot_curve_3d(project_to_y(wave,-1.1f), 0.0,length, 4*std::max(1,int(length/lambda)))
		.stroke(rgb(1.0,0.6,0)).stroke_width(0.02*power_x);
	graph.plot_curve_3d(project_to_z(wave,-0.1f), 0.0,length, 4*std::max(1,int(length/lambda)))
		.stroke(rgb(0.2,0.2,0.8)).stroke_width(0.02*power_x);
	graph.plot_curve_3d(wave, 0.0,length,4*std::max(1,int(length/lambda)))
		.stroke(rgb(0.9,0.9,0.0)).stroke_width(0.04*power_x);
	
	std::fstream f(filename, std::ofstream::out | std::ofstream::trunc);
	f<<svg.automatic_viewBox();
}
