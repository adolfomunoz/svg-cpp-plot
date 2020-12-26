#include <array>
#include <cmath>

namespace svg_cpp_plot {
namespace axis_scale {
  
class Base {
public:
    virtual float transform(float x)     const noexcept = 0;
    virtual float antitransform(float y) const noexcept = 0;
};

class linear : public Base {
public:
    float transform(float x)     const noexcept override { return x; }
    float antitransform(float y) const noexcept override { return y; }
};

class log : public Base {
public:
    float transform(float x)     const noexcept override { return std::log(x); }
    float antitransform(float y) const noexcept override { return std::exp(y); }
};

class symlog : public Base {
    float linear_threshold;
public:
    symlog(float l) : linear_threshold(l) {}
    symlog()        : symlog(0.01)        {}
    float transform(float x)     const noexcept override {
        return (x>linear_threshold)?(std::log(x-linear_threshold)+linear_threshold):
                    ((x>(-linear_threshold))?x:(-linear_threshold-std::log(linear_threshold-x)));
    }
    float antitransform(float y)     const noexcept override {
        return (y>linear_threshold)?(std::exp(y-linear_threshold)-linear_threshold):
                    ((y>(-linear_threshold))?y:(linear_threshold-std::exp(linear_threshold-y)));
    }    
};
}    
    
};