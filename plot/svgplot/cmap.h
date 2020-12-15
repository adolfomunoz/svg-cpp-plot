#pragma once

namespace svg_cpp_plot {
namespace detail {
    float vmax_element(float f) { return f; }
    float vmin_element(float f) { return f; }
    float vmax_element(const std::tuple<float,float,float>& f) { return std::max(std::get<0>(f),std::max(std::get<1>(f),std::get<2>(f))); }
    float vmin_element(const std::tuple<float,float,float>& f) { return std::min(std::get<0>(f),std::min(std::get<1>(f),std::get<2>(f))); }  
    float vmax_element(const std::tuple<float,float,float,float>& f) { return std::max(std::get<3>(f),std::max(std::get<0>(f),std::max(std::get<1>(f),std::get<2>(f)))); }
    float vmin_element(const std::tuple<float,float,float,float>& f) { return std::min(std::get<3>(f),std::min(std::get<0>(f),std::min(std::get<1>(f),std::get<2>(f)))); } 

    template<typename R>
    void set_color(R& rect, const _2d::color_map& cm, float v) noexcept {
        rect.fill(rgb(cm(v)));
    }
    
    template<typename R>
    void set_color(R& rect, const _2d::color_map& cm, const std::tuple<float,float,float>& v) noexcept {
        rect.fill(rgb(v));
    }
   
    template<typename R>
    void set_color(R& rect, const _2d::color_map& cm, const std::tuple<float,float,float,float>& v) noexcept {
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
    
    template<typename T>
    float opacity_of(const T& t, const _2d::color_map& cm) noexcept { return 1; }
    
    float opacity_of(const std::tuple<float,float,float,float>& f, const _2d::color_map& cm) noexcept { 
        return std::get<3>(f); 
    }
    
    _2d::color_map colormap(std::string_view cmap, float vmin, float vmax) {
        _2d::color_map cm = _2d::color_map_viridis(vmin,vmax);
		if (cmap == "magma") 
			cm = _2d::color_map_magma(vmin,vmax);
		else if (cmap == "grayscale") 
			cm = _2d::color_map_grayscale(vmin,vmax);
		else if (cmap == "inferno") 
			cm = _2d::color_map_inferno(vmin,vmax);
		else if (cmap == "plasma") 
			cm = _2d::color_map_plasma(vmin,vmax);
		else if (cmap == "bwr") 
			cm = _2d::color_map_bwr(vmin,vmax);
		else if (cmap == "seismic") 
			cm = _2d::color_map_seismic(vmin,vmax);
		else if (cmap == "coolwarm") 
			cm = _2d::color_map_coolwarm(vmin,vmax);
		else if (cmap == "Spectral") 
			cm = _2d::color_map_spectral(vmin,vmax);
		else if (cmap == "PiYG") 
			cm = _2d::color_map_piyg(vmin,vmax);
        return cm;
    }
};
};