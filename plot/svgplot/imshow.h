#pragma once

#include <list>
#include <tuple>
#include <string>
#include "../../2d/function-2d.h"

namespace svg_cpp_plot {
    
class ImShow : public Generator<_2d::Matrix> {
	std::string cmap_;
	float vmin_, vmax_; bool vmin_set, vmax_set;
    std::string interpolation_;
    std::array<float,4>  extent_; bool extent_set;
    
public:
    ImShow() : vmin_set(false), vmax_set(false), extent_set(false) {}
	ImShow& vmin(float f) { vmin_=f; vmin_set=true; return (*this);}
	ImShow& vmax(float f) { vmax_=f; vmax_set=true; return (*this);}
    ImShow& extent(const std::array<float,4> & e) { extent_=e; extent_set=true; return (*this); }
    
protected:
    virtual float calculated_vmin() const = 0;
    virtual float calculated_vmax() const = 0;
    



public:
 	float vmin() const {
		if (vmin_set) return vmin_;
        else return calculated_vmin();
    }
    
 	float vmax() const {
		if (vmax_set) return vmax_;
        else return calculated_vmax();
    }
   
	ImShow& cmap(std::string_view s) { cmap_=s; return *this; }
	std::string_view cmap() const { return cmap_; }
	ImShow& interpolation(std::string_view s) { interpolation_=s; return *this; }
	std::string_view interpolation() const { return interpolation_; }
    
	virtual std::tuple<std::size_t,std::size_t> size() const = 0;
    
	std::array<float,4> axis() const {
        if (extent_set) return extent_;
        else return std::array<float,4>{
				-0.5f,float(std::get<0>(size()))-0.5f,
				-0.5f,float(std::get<1>(size()))-0.5f};
	}
    
    std::array<float,4> extent() const {
        return axis();
    }
    
    
protected:
    _2d::color_map colormap() const {
        _2d::color_map cm = _2d::color_map_viridis(vmin(),vmax());
		if (cmap() == "magma") 
			cm = _2d::color_map_magma(vmin(),vmax());
		else if (cmap() == "grayscale") 
			cm = _2d::color_map_grayscale(vmin(),vmax());
        return cm;
    }
};

namespace {
    float vmax_element(float f) { return f; }
    float vmin_element(float f) { return f; }
    float vmax_element(const std::tuple<float,float,float>& f) { return std::max(std::get<0>(f),std::max(std::get<1>(f),std::get<2>(f))); }
    float vmin_element(const std::tuple<float,float,float>& f) { return std::min(std::get<0>(f),std::min(std::get<1>(f),std::get<2>(f))); }  
    float vmax_element(const std::tuple<float,float,float,float>& f) { return std::max(std::get<3>(f),std::max(std::get<0>(f),std::max(std::get<1>(f),std::get<2>(f)))); }
    float vmin_element(const std::tuple<float,float,float,float>& f) { return std::min(std::get<3>(f),std::min(std::get<0>(f),std::min(std::get<1>(f),std::get<2>(f)))); } 

    template<typename R>
    void set_rectangle(R& rect, const _2d::color_map& cm, float v) noexcept {
        rect.fill(rgb(cm(v)));
    }
    
    template<typename R>
    void set_rectangle(R& rect, const _2d::color_map& cm, const std::tuple<float,float,float>& v) noexcept {
        rect.fill(rgb(v));
    }
   
    template<typename R>
    void set_rectangle(R& rect, const _2d::color_map& cm, const std::tuple<float,float,float,float>& v) noexcept {
        rect.fill(rgb(std::get<0>(v),std::get<1>(v),std::get<2>(v))).opacity(std::get<3>(v));
    }   

    
    std::tuple<float,float,float> color_of(float f, const _2d::color_map& cm) noexcept {
        return cm(f);
    }
    
    std::tuple<float,float,float> color_of(const std::tuple<float,float,float>& f, const _2d::color_map& cm) noexcept {
        return f;
    }
      
    //TODO: Transparency
    std::tuple<float,float,float> color_of(const std::tuple<float,float,float,float>& f, const _2d::color_map& cm) noexcept {
        return std::tuple(std::get<0>(f),std::get<1>(f),std::get<2>(f));
    }
 
};

template<typename T>
class ImShowType : public ImShow {
    std::vector<std::vector<T>> data;

public:
    ImShowType(const std::vector<std::vector<T>>& data) : data(data) {}
    ImShowType(std::vector<std::vector<T>>& data) : data(std::forward<std::vector<std::vector<T>>>(data)) {}
protected:
    float calculated_vmin() const override {
        float temp_vmin = vmin_element(data[0][0]);
        for (const auto& row : data) for (const auto& value : row) {
            float v = vmin_element(value);
            if (v < temp_vmin) temp_vmin = v;
        }
        return temp_vmin;       
    }
    
    float calculated_vmax() const override {
        float temp_vmax = vmax_element(data[0][0]);
        for (const auto& row : data) for (const auto& value : row) {
            float v = vmax_element(value);
            if (v > temp_vmax) temp_vmax = v;
        }
        return temp_vmax;         
    }
public:
    std::tuple<std::size_t,std::size_t> size() const override {
		std::size_t x(0);
		for (const auto& r : data) if (r.size()>x) x=r.size();
		return std::tuple<std::size_t,std::size_t>(x,data.size());        
    }
    
private:
    _2d::Group nearest() const noexcept {
        auto cm = this->colormap();
        auto image = _2d::group();
        const float border_expansion=0;
		image.stroke_width(0);
        std::tuple<float,float> xmin{axis()[0],axis()[2]};
        std::tuple<float,float> xmax{axis()[1],axis()[3]};
        auto nsamples = size();
		float dx = (std::get<0>(xmax)-std::get<0>(xmin))/float(std::get<0>(nsamples));
		float dy = (std::get<1>(xmax)-std::get<1>(xmin))/float(std::get<1>(nsamples));
		float x, y;
		const float eps = border_expansion*dx;
        
        y = std::get<1>(xmin);
        for (const auto& row : data) {
            x = std::get<0>(xmin);
            for (const auto& v : row) {
                set_rectangle(image.add(_2d::rect({x-eps,y-eps},{x+dx+eps,y+dy+eps})).stroke_width(0),cm,v);
                x+=dx;
            }
            y+=dy;
        }

        return image;
    }
    
    auto interpolated() const noexcept {
        auto cm = this->colormap();
        auto nsamples = size();
		PNG png(std::get<0>(nsamples),std::get<1>(nsamples));
		unsigned int i, j = 0;
        for (const auto& row : data) {
            i=0;
            for (const auto& v : row) {
                png.set_pixel(i,j,color_of(v,cm)); ++i; 
            }
            ++j;
        }
		png.save("tmp.png");

		std::ifstream stream("tmp.png", std::ios::in | std::ios::binary);
		std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
		std::stringstream ss;
		ss << "data:image/png;base64,"<<base64_encode(contents.data(),contents.size());
        std::tuple<float,float> xmin{axis()[0],axis()[2]};
        std::tuple<float,float> xmax{axis()[1],axis()[3]};
		Image image(ss.str()); image.rect(xmin,xmax);
        return _2d::primitive(image);
    }
    
    std::string to_string(const _2d::Matrix& m) const noexcept override {
        if ((interpolation()=="") || (interpolation()=="nearest") ||(interpolation()=="none")) 
            return nearest().to_string(m);
        else
            return interpolated().to_string(m);
    }
};


	
}