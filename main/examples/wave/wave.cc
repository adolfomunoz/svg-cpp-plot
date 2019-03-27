#include "../../../svg-cpp-plot.h"
#include <fstream>
#include <cstring>
#include <vector>

using namespace svg_cpp_plot;

int main(int argc, char** argv) {
	std::vector<float> lambdas; lambdas.push_back(1.0f);
	std::vector<float> powers_x; powers_x.push_back(1.0f);
	std::vector<float> powers_y; powers_y.push_back(1.0f);
	std::vector<float> phases_x; phases_x.push_back(0.0f);
	std::vector<float> phases_y; phases_y.push_back(M_PI/2.0f);
	float length = 6.0;
	const char* filename = "output.svg";
	
	for (int i=0;i<argc-1;++i) {
		if (strcmp(argv[i],"-lambda")==0)       {    ++i; lambdas.clear(); for(;(i<argc)&&(argv[i][0]!='-');++i) { lambdas.push_back(atof(argv[i]));}               --i; }
		else if (strcmp(argv[i],"-power-x")==0) {    ++i; powers_x.clear(); for(;(i<argc)&&(argv[i][0]!='-');++i) { powers_x.push_back(atof(argv[i]));}             --i; }
		else if (strcmp(argv[i],"-power-y")==0) {    ++i; powers_y.clear(); for(;(i<argc)&&(argv[i][0]!='-');++i) { powers_y.push_back(atof(argv[i]));}             --i; }
		else if (strcmp(argv[i],"-phase-x")==0) {    ++i; phases_x.clear(); for(;(i<argc)&&(argv[i][0]!='-');++i) { phases_x.push_back(M_PI*atof(argv[i])/180.0f);} --i; }
		else if (strcmp(argv[i],"-phase-y")==0) {    ++i; phases_y.clear(); for(;(i<argc)&&(argv[i][0]!='-');++i) { phases_y.push_back(M_PI*atof(argv[i])/180.0f);} --i; }
		else if (strcmp(argv[i],"-length")==0) length = atof(argv[++i]);
		else if (strcmp(argv[i],"-filename")==0) filename = argv[++i];
	}
	
	SVG svg;
	Graph3D& graph = svg.add(Graph3D(matrix3::oblique*matrix3::zyx));

	float max_power_x = 0.0;
	for (float power_x: powers_x) if (power_x > max_power_x) max_power_x = power_x;
	float avg_power_x = 0.0;
	for (float power_x: powers_x) avg_power_x+=power_x;
	avg_power_x/=powers_x.size();

	float max_power_y = 0.0;
	for (float power_y: powers_y) if (power_y > max_power_y) max_power_y = power_y;
	
	graph.arrow({0.0f,0.0f,0.0f},{0.0f,0.0f,length*1.2f},0.04f*avg_power_x)
		.stroke(grey).stroke_width(0.04*avg_power_x);

	float minx = -max_power_x - 0.1f;
	float maxx = max_power_x + 0.1f;
	float miny = -max_power_y - 0.1f;
	float maxy = max_power_y + 0.1f;
	float minz = -0.1f;
	float maxz = length + 0.1f;

	graph.polyline({{maxx,maxy,minz},{minx,maxy,minz},{minx,miny,minz},{maxx,miny,minz},{maxx,maxy,minz}})
		.stroke(black).stroke_width(0.02*avg_power_x);
	graph.polyline({{maxx,maxy,minz},{maxx,miny,minz},{maxx,miny,maxz},{maxx,maxy,maxz},{maxx,maxy,minz}})
		.stroke(black).stroke_width(0.02*avg_power_x);
	graph.polyline({{maxx,miny,minz},{minx,miny,minz},{minx,miny,maxz},{maxx,miny,maxz},{maxx,miny,minz}})
		.stroke(black).stroke_width(0.02*avg_power_x);

	std::size_t max_index = std::max(lambdas.size(),std::max(powers_x.size(),std::max(powers_y.size(),std::max(phases_x.size(),phases_y.size()))));
	for (std::size_t i = 0;i<max_index;++i) {
		float lambda = lambdas[std::min(i,lambdas.size()-1)];
		float power_x = powers_x[std::min(i,powers_x.size()-1)];
		float power_y = powers_y[std::min(i,powers_y.size()-1)];
		float phase_x = phases_x[std::min(i,phases_x.size()-1)];
		float phase_y = phases_y[std::min(i,phases_y.size()-1)];
		std::cout<<"Wave: lambda = "<<lambda<<" - power_x = "<<power_x<<" - power_y = "<<power_y<<" - phase_x = "<<phase_x<<" - phase_y = "<<phase_y<<std::endl;
		auto wave = [&] (float t) { return std::tuple(power_x*sin(2*M_PI*t/lambda + phase_x),
						power_y*sin(2*M_PI*t/lambda + phase_y),t); };
		graph.plot_curve_3d(project_to_x(wave,maxx), 0.0,length, 4*std::max(1,int(length/lambda)))
			.stroke(rgb(0.5,0.8,0)).stroke_width(0.02*avg_power_x);
		graph.plot_curve_3d(project_to_y(wave,miny), 0.0,length, 4*std::max(1,int(length/lambda)))
			.stroke(rgb(1.0,0.6,0)).stroke_width(0.02*avg_power_x);
		graph.plot_curve_3d(project_to_z(wave,minz), 0.0,length, 4*std::max(1,int(length/lambda)))
			.stroke(rgb(0.2,0.2,0.8)).stroke_width(0.02*avg_power_x);
		graph.plot_curve_3d(wave, 0.0,length,4*std::max(1,int(length/lambda)))
			.stroke(rgb(0.9,0.9,0.0)).stroke_width(0.04*avg_power_x);
	}

	
	std::fstream f(filename, std::ofstream::out | std::ofstream::trunc);
	f<<svg.automatic_viewBox();
}
