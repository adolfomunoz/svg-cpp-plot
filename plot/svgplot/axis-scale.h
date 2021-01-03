#include <array>
#include <cmath>
#include "exceptions.h"

namespace svg_cpp_plot {
namespace axis_scale {
  
class Base {
public:
    virtual float transform(float x)     const noexcept = 0;
    virtual float antitransform(float y) const noexcept = 0;
    virtual Base& base(float b)      { throw exception_invalid_parameter(*this,"base"); }
    virtual Base& linthresh(float b) { throw exception_invalid_parameter(*this,"linthresh"); }
    virtual Base& linscale(float b)   { throw exception_invalid_parameter(*this,"linscale"); }
    virtual bool is_valid(float x) const noexcept { return true; }
    std::vector<float> filter(const std::vector<float>& sol, float threshold) const noexcept {
        std::vector<float> filtered_sol;
        for (float x : sol) {
            if ( (std::abs(transform(sol.back()) - transform(x))>threshold) &&
                    (filtered_sol.empty() || (std::abs(transform(filtered_sol.back()) - transform(x))>threshold) ) ) 
                filtered_sol.push_back(x);
        }
        filtered_sol.push_back(sol.back()); 
        return filtered_sol;
    }
    virtual std::vector<float> ticks(int target_ticks, float xmin, float xmax) const noexcept {
        float tick_step = std::floor((xmax - xmin)/float(target_ticks));
		int factor = 2;
		while (tick_step<=0.0f) {
			tick_step=std::floor(factor*(xmax - xmin)/float(target_ticks))/float(factor);
			if ((factor % 4) == 0) factor = (factor*10)/4;
			else factor*=2;
		}
		float first_tick = std::ceil(xmin/tick_step)*tick_step;
		std::vector<float> sol;
		for (float x = first_tick; x <= xmax; x+=tick_step) if (is_valid(x)) sol.push_back(x);
		return filter(sol,(transform(sol.back()) - transform(sol.front()))/target_ticks) ;
    }
};

class linear : public Base {
public:
    float transform(float x)     const noexcept override { return x; }
    float antitransform(float y) const noexcept override { return y; }
};

class log : public Base {
    float base_;
public:
    log(float base = 10.0f) : base_(base) {}
    log& base(float b) override { base_ = b; return (*this); }
    float base() const { return base_; }
    bool is_valid(float x)       const noexcept override { return x>0; }
    float transform(float x)     const noexcept override { return std::log((x>0)?x:1.e-3f)/std::log(base()); }
    float antitransform(float y) const noexcept override { return std::pow(base(),y); }
};

class symlog : public Base {
    float linthresh_;
    float linscale_;     
    log l;
public:
    symlog(float lt = 0.01f, float ls = 1.0f, float b = 10.0f) : linthresh_(lt), linscale_(ls), l(b) {}
    symlog& base(float b) override { l.base(b); return (*this); }
    float base() const { return l.base(); }
    float linthresh() const { return linthresh_; }
    symlog& linthresh(float l) { linthresh_ = l; return (*this); }
    float linscale() const { return linscale_; }
    symlog& linscale(float l) { linscale_ = l; return (*this); }
   
    float transform(float x)     const noexcept override {        
        float linscale_adj = linscale() / (1.0f - 1.0f/l.base());
        if (std::abs(x)<linthresh()) return x*linscale_adj;
        else {
            float sign = (x>=0)?1.0f:-1.0f;
            return sign * linthresh()* (linscale_adj + l.transform(std::abs(x) / linthresh()));
        }
    }
    
    float antitransform(float y)     const noexcept override {
        float linscale_adj = linscale() / (1.0f - 1.0f/l.base());
        if (std::abs(y) < transform(linthresh())) return y/linscale_adj;
        else {
            float sign = (y>=0)?1.0f:-1.0f;
            return sign * linthresh()* l.antitransform(std::abs(y)/linthresh() - linscale_adj);
        }
    }    
};
}    
   
axis_scale::log log;
axis_scale::symlog symlog;
axis_scale::linear linear;
   
};