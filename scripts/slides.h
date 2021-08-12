#pragma once

#include "script.h"

namespace svg_cpp_plot {
namespace script {
class Slides : public Code {
	std::string slide_class;
	float transition_time;
public:
	Slides(const std::string& slide_class = "slide", float transition_time = 0.2f) : 
        slide_class(slide_class), transition_time(transition_time) { }

    std::string init_function() const { return "init"; }
	std::string to_string() const noexcept override {
		std::stringstream output;
        output << "var slidenumber=0;"<<std::endl;
        output << "function switchSlide(evt,slidenum) {"<<std::endl;
        output << "    var svgDocument = evt.target.ownerDocument;"<<std::endl;
        output << "    var svgRoot = svgDocument.documentElement;"<<std::endl;
        output << "    var slideAway = svgRoot.getElementById(\""<<slide_class<<"\"+slidenumber);"<<std::endl;
        output << "    var slideIn = svgRoot.getElementById(\""<<slide_class<<"\"+slidenum);"<<std::endl;
        output << "    if (slideIn) {"<<std::endl;
        output << "        if (slideAway) {"<<std::endl;
        output << "            if (slidenum>slidenumber) {"<<std::endl;
        output << "                slideAway.setAttribute(\"style\",\"\");"<<std::endl;
        output << "                setTimeout(function(){"<<std::endl;
        output << "                    slideAway.setAttribute(\"fill-opacity\",0.0);"<<std::endl;
        output << "                },"<<(1000.0f*transition_time)<<");              "<<std::endl;
        output << "            }"<<std::endl;
        output << "            else {"<<std::endl;
        output << "                slideAway.setAttribute(\"style\",\"transition: fill-opacity "<<transition_time<<"s\");"<<std::endl;
        output << "                slideAway.setAttribute(\"fill-opacity\",0.0);"<<std::endl;
        output << "            }                    "<<std::endl;
        output << "        }"<<std::endl;
        output << "        if (slidenum>slidenumber) slideIn.setAttribute(\"style\",\"transition: fill-opacity "<<transition_time<<"s\");"<<std::endl;
        output << "        else slideIn.setAttribute(\"style\",\"\");"<<std::endl;
        output << "        slideIn.setAttribute(\"fill-opacity\",1.0);"<<std::endl;       
        output << "        slidenumber=slidenum;"<<std::endl;
        output << "    }"<<std::endl;
        output << "}"<<std::endl;
        output << "function init(evt) {"<<std::endl;
        output << "    var svgDocument = evt.target.ownerDocument;"<<std::endl;
        output << "    svgDocument.addEventListener(\"keydown\", function(ev) {"<<std::endl;
        output << "        if (ev.keyCode == '37') switchSlide(ev,slidenumber-1);"<<std::endl;
        output << "        else if (ev.keyCode == '39') switchSlide(ev,slidenumber+1);"<<std::endl;
        output << "    });"<<std::endl;
        output << "    var svgRoot = svgDocument.documentElement;"<<std::endl;
        output << "    var slides = svgRoot.getElementsByClassName(\""<<slide_class<<"\");"<<std::endl;
        output << "    for (var i = 0; i < slides.length; ++i) {"<<std::endl;
        output << "        slides[i].setAttribute(\"fill-opacity\",0.0);"<<std::endl;
        output << "        slides[i].setAttribute(\"id\",\""<<slide_class<<"\"+(i+1));"<<std::endl;
        output << "    }"<<std::endl;
        output << "    switchSlide(evt,1);"<<std::endl;
        output << "}"<<std::endl;

		return output.str();
	}
};	

}
}
