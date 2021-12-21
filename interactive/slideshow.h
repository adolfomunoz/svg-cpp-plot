#include "../scripts/slides.h"
#include "../primitives/svg.h"

namespace svg_cpp_plot {

class SlideShow : public SVG {
    std::string slide_class; 
public:
    SlideShow(const std::string& s = "slide", float transition = 0.2) : slide_class(s) {
        script::Slides scr(s,transition);
        this->SVG::add(Script()).add(scr);
        this->set("onload",scr.init_function()+std::string("(evt)"));
    }
    
    template<typename E>
    E& add(const E& e) {
        E& inserted_e = SVG::add(e);
        inserted_e.class_().add(slide_class);
        return inserted_e;
    }
};
    
}