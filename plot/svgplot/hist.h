#pragma once

#include <list>
#include <tuple>
#include <string>
#include "bar.h"
#include "barh.h"
#include "plot.h"
#include "../../2d/transform.h"
#include "../../2d/points.h"
#include "../../2d/polyline.h"
#include "color.h"
#include <algorithm>

namespace svg_cpp_plot {
    
enum class Orientation { vertical, horizontal };
enum class HistType { bar, barstacked, step, stepfilled };

class Hist : public Plottable  {
    std::vector<float> x_; //Todo: enable multiple data (std::vector<std::vector<float>>)
    std::size_t bins_value;
    std::vector<float> bins_; bool bins_set;
    std::vector<float> weights_;
    std::tuple<float,float> range_; bool range_set;
    bool density_, cumulative_;
    Orientation orientation_;
    HistType histtype_;

    std::shared_ptr<Color> color_;
    float alpha_;
public:
	Hist(const std::vector<float>& x) : x_(x), bins_value(10), bins_set(false),weights_(1,1.0f), range_set(false),density_(false),cumulative_(false),orientation_(Orientation::vertical),histtype_(HistType::bar),alpha_(1) {}
     
    std::tuple<float,float> range() const {
        if (range_set) return range_;
        else if (bins_set) return std::tuple<float,float>(bins_.front(),bins_.back());
        else return std::tuple<float,float>(*std::min_element(x_.begin(),x_.end()),*std::max_element(x_.begin(),x_.end()));
    }
    
    Hist& range(const std::tuple<float,float> r) {
        range_=r; range_set=true; return *this;
    }
    
    Hist& weights(const std::vector<float>& w) { 
        weights_=w; return *this; 
    }
    
private:       
    std::size_t bins_size() const {
        if (bins_set) return bins_.size()-1;
        else return bins_value;
    }

    float bin(std::size_t i) const {
        if (bins_set) return bins_[i];
        else {
            auto [xmin,xmax] = range();
            return xmin+float(i)*(xmax-xmin)/float(bins_size());
        }
    }
    
public:
    Hist& orientation(const Orientation& o) {
        orientation_ = o; return *this;
    }
    
    Hist& orientation(const std::string_view& s) {
        if (s[0]=='h') return this->orientation(Orientation::horizontal);
        else return this->orientation(Orientation::vertical);
    }
    
    Hist& histtype(const HistType& h) {
        histtype_ = h; return *this;
    }
    
    Hist& histtype(const std::string_view& s) {
        if (s[0]=='b') return this->histtype(HistType::bar);
        else return this->histtype(HistType::step);
    }
    

    
    
    
private:
    float weight_(int i) const {
        return weights_[i%weights_.size()];
    }
    
public:
    Hist& density(bool b = true) { density_ = b; return *this;}
    Hist& cumulative(bool c = true) { cumulative_=c; return *this; }
    
    Hist& bins(std::size_t b) { bins_value=b; return *this;}
    Hist& bins(int b) { return bins(std::size_t(b)); }
    Hist& bins(const std::vector<float>& b) { bins_=b; bins_set=true; return *this; }
    template<typename Collection>
    Hist& bins(const Collection& c) { return bins(std::vector<float>(c.begin(),c.end())); }
    
    std::vector<float> hist_values() const {
        std::vector<float> hist(bins_size(),0.0f);
        float w = 0.0f;
        for (std::size_t ix=0; ix<x_.size(); ++ix) {
            w += weight_(ix);
            for (std::size_t ib = 0; ib<(bins_size()-1); ++ib) {
                if ((x_[ix] >= bin(ib)) && (x_[ix] < bin(ib+1)))
                    hist[ib]+=weight_(ix);
            }
            if ((x_[ix] >= bin(bins_size()-1)) && (x_[ix] <= bin(bins_size())))
                    hist[bins_size()-1]+=weight_(ix);
        }
        if (density_) for (auto& h : hist) h/=w;
        if (cumulative_) {
            float c = 0.0f;
            for (auto& h : hist) {
                c += h; h = c;
            }
        }
        return hist;
    }
    
    std::vector<float> hist_positions() const {
        std::vector<float> hist(bins_size(),0.0f);
        for (std::size_t ib = 0; ib<bins_size(); ++ib)
            hist[ib]=0.5*(bin(ib)+bin(ib+1));
        return hist;
    }

    std::vector<float> hist_widths() const {
        std::vector<float> hist(bins_size(),0.0f);
        for (std::size_t ib = 0; ib<bins_size(); ++ib)
            hist[ib]=bin(ib+1)-bin(ib);
        return hist;
    }  
    
    Hist& alpha(float f) { alpha_=f; return *this; }
    float alpha() const { return alpha_; }


    template<typename C>
	Hist& color(const C& c) { color_=detail::color(c); return *this; }
	const Color& color() const { return color_?*color_:black; }
   
	std::array<float,4> axis() const override {
        float bmin = bin(0); float bmax = bin(bins_size());
        std::vector<float> h = hist_values();
        float hmax = 1.1f*(*std::max_element(h.begin(),h.end()));
        if (orientation_ == Orientation::vertical)
            return std::array<float,4>{bmin,bmax,0.0f,hmax};
        else
            return std::array<float,4>{0.0f,hmax,bmin,bmax};
	}
    
	std::string to_string(const _2d::Matrix& m) const noexcept override {
        if (histtype_==HistType::bar) {
            if (orientation_==Orientation::vertical)
                return Bar(hist_positions(),hist_values()).width(hist_widths()).color(color_).alpha(alpha()).to_string(m);
            else 
                return BarH(hist_positions(),hist_values()).height(hist_widths()).color(color_).alpha(alpha()).to_string(m);    
        } else { // histtype_==HistType::step
            if (orientation_==Orientation::vertical)
                return Plot(hist_positions(),hist_values()).color(color_).alpha(alpha()).to_string(m);
            else    
                return Plot(hist_values(),hist_positions()).color(color_).alpha(alpha()).to_string(m);
        }
	}

};

	
}
