#pragma once
#include <cstdlib>
#include <string>

namespace svg_cpp_plot {

class cannot_browse : public std::exception {
public: 
	const char* what() const noexcept override { return "Do not know how to browse url"; }
};


void browse(const char* url) {
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
		if (0 > system((std::string("start google-chrome ")+url).c_str()))
		if (0 > system((std::string("start firefox")+url).c_str()))
		throw cannot_browse();
	#else
		if (0 > system((std::string("google-chrome ")+url+" &").c_str()))
		if (0 > system((std::string("firefox ")+url+" &").c_str()))
		throw cannot_browse();
	#endif
}	
}
