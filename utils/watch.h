#pragma once
#include "../core/svg.h"


namespace svg_cpp_plot {
void browse(const char* url) {
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
		system((std::string("start google-chrome ")+url).c_str());
	#else
		system((std::string("google-chrome ")+url+" &").c_str());
	#endif
}	

