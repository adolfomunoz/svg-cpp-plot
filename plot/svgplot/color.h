#pragma once
#include <memory>
#include "../../attributes/color.h"

namespace svg_cpp_plot {
namespace detail {

    std::shared_ptr<Color> color(const std::shared_ptr<Color>& c) {
        return c;
    }
    
    template<typename C>
    std::shared_ptr<Color> color(const C& c) {
        return std::make_shared<C>(c);
    }
    
    std::shared_ptr<Color> color(const std::string& s) {
        if (!s.empty()) {
            if (s[0]=='#') return std::make_shared<color_hex>(s.substr(1));
            else if (s.size()==1) {
                switch (s[0]) {
					case 'r': return std::make_shared<redColor>();
					case 'g': return std::make_shared<greenColor>();
					case 'b': return std::make_shared<blueColor>();
					case 'k': return std::make_shared<blackColor>(); 
					case 'w': return std::make_shared<whiteColor>();
					case 'c': return std::make_shared<cyanColor>();
					case 'm': return std::make_shared<magentaColor>();
					case 'y': return std::make_shared<yellowColor>();
				}
            } else return std::make_shared<NamedColor>(s);
        }
        return std::make_shared<blackColor>();
    }
    
    std::shared_ptr<Color> color(const char* s) { return color(std::string(s)); }
    
    std::vector<std::shared_ptr<Color>> color_vector(const std::vector<std::string>& vs) {
        std::vector<std::shared_ptr<Color>> res;
        for (const std::string& s : vs) res.push_back(color(s)); 
        return res;
    }
    
    std::vector<std::shared_ptr<Color>> color_vector(const std::vector<rgb>& vs) {
        std::vector<std::shared_ptr<Color>> res;
        for (const rgb& s : vs) res.push_back(color(s)); 
        return res;
    }
    
//    template<typename C>
//    std::vector<std::shared_ptr<Color>> color_vector(const C& vs,
//            typename std::enable_if<std::is_same<std::decay<typename C::value_type>::type,std::string>::value>::type* dummy = 0) {
//                return color_vector(std::vector<std::string>(vs.begin(),vs.end());
//    }
    
    template<typename C>
    std::vector<std::shared_ptr<Color>> color_vector(const C& vs) {
        std::vector<std::shared_ptr<Color>> res;
        res.push_back(color(vs));
        return res;
    }
    
    
        

};    
};