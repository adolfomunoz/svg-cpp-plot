#include "../scripts/slides.h"
#include "../primitives/svg.h"

namespace svg_cpp_plot {

class SlideShow : public SVG {
    std::string slide_class; 
public:
    SlideShow(const std::string& s = "slide", float transition = 0.2) : slide_class(s) {
        script::Slides scr(s,transition);
        this->add(Script()).add(scr);
        this->set("onload",scr.init_function()+std::string("(evt)"));
    }
    
    void on_element_add(Element& e) { if (e.tag() != "script") e.get_or_set("class",slide_class); }
};
    
}