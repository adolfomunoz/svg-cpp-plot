#pragma once

#include <list>
#include <tuple>
#include <fstream>
#include <memory>
#include "../primitives/svg.h"
#include "../2d/transform.h"
#include "../2d/polygon.h"
#include "../2d/point-list.h"
#include "../2d/polyline.h"
#include "../2d/text.h"
#include "../attributes/style.h"
#include "svgplot/arange.h"
#include "svgplot/plot.h"
#include "svgplot/imshow.h"
#include "svgplot/bar.h"
#include "svgplot/barh.h"
#include "svgplot/hist.h"
#include "svgplot/scatter.h"

namespace svg_cpp_plot {
    
namespace { //Anonymous namespace, only visible from this .h
	template<std::size_t N>
	std::array<float,N>& operator+=(std::array<float,N>& a, const std::array<float,N>& b) {
		for (std::size_t i = 0; i<N; ++i) a[i]+=b[i];
		return a;
	}
    
 	template<std::size_t N>
    std::array<float,N> operator+(const std::array<float,N>& x, const std::array<float,N>& y) {
        std::array<float,N> sol = x;
        return sol+=y;
    }
    
}

class Font {
    float size_;
public:
    Font(float size = 10) : size_(size) {}
    Font& fontsize(float size) {
        size_=size; return (*this);
    }
    float fontsize() const { return size_; }
};

		
class SVGPlot {
	std::array<float,2> figsize_; bool figsize_set;
	std::vector<std::shared_ptr<Color>> cycle; std::size_t cycle_pos;

	std::string ylabel_, xlabel_, title_;
    Font ylabel_font, xlabel_font, title_font, xticklabels_font, yticklabels_font;
	std::array<float,4> axis_; bool axis_set;
	
	std::vector<std::string> xticklabels_; bool xticklabels_set;
	std::vector<float> xticks_; bool xticks_set;
	std::vector<std::string> yticklabels_; bool yticklabels_set;
	std::vector<float> yticks_; bool yticks_set;
    
    float linewidth_;
	
	
    std::list<std::shared_ptr<Plottable>> plottables;
    std::unique_ptr<axis_scale::Base> xscale_, yscale_;
	
	
	std::vector<std::unique_ptr<SVGPlot>> subplots_;
    int nrows, ncols;
    
    class SubplotsAdjust {
        float left_, right_, bottom_, top_, wspace_, hspace_;
    public:
        SubplotsAdjust() : left_(0.01), right_(0.99), bottom_(0.01), top_(0.99), wspace_(0.01), hspace_(0.01) {}
        SubplotsAdjust& left(float v) { left_=v; return *this; }
        SubplotsAdjust& right(float v) { right_=v; return *this; }
        SubplotsAdjust& top(float v) { top_=v; return *this; }
        SubplotsAdjust& bottom(float v) { bottom_=v; return *this; }
        SubplotsAdjust& wspace(float v) { wspace_=v; return *this; }
        SubplotsAdjust& hspace(float v) { wspace_=v; return *this; }
        float left() const { return left_; }       
        float right() const { return right_; }       
        float top() const { return top_; }       
        float bottom() const { return bottom_; }       
        float wspace() const { return wspace_; }       
        float hspace() const { return hspace_; }       
    } subplots_adjust_;
	SVGPlot* parent; 
    
    unsigned long target_xticks, target_yticks;
private:
    float subplots_xmax(int c) const {
        float x = 0;
        for (int r = 0; r<nrows; ++r)
            if (const auto& sp = subplots_[r*ncols+c])
                x = std::max(sp->figsize()[0],x);
        return x;
    }
    
    float subplots_ymax(int r) const {
        float y = 0;
        for (int c = 0; c<ncols; ++c)
            if (const auto& sp = subplots_[r*ncols+c])
                y = std::max(sp->figsize()[1],y);
        return y;
    }
    
    std::array<float,2> subplot_size() const {
        float width = 0, height = 0;
        for (int r = 0; r<nrows; ++r) height += subplots_ymax(r);
        for (int c = 0; c<ncols; ++c) width += subplots_xmax(c);
        return std::array<float,2>{width*(1.0f+(ncols-1)*subplots_adjust_.wspace()/float(ncols)),height*(1.0f+(nrows-1)*subplots_adjust_.hspace()/float(nrows))};
    }
    
    std::array<float,4> yticks_margin(const std::array<float,4>& ax) const {
        if (subplots_.empty())
            return (!yticklabels(ax).empty())?
                    std::array<float,4>{yticklabels_font.fontsize()*2.5f,0,0,0}:
                    ((!yticks(ax).empty())?
                        std::array<float,4>{3,0,0,0}:
                        std::array<float,4>{0,0,0,0});
        else return std::array<float,4>{0,0,0,0};
    }
    
    std::array<float,4> xticks_margin(const std::array<float,4>& ax) const {
        if (subplots_.empty())
            return (!xticklabels(ax).empty())?
                    std::array<float,4>{0,0,0,xticklabels_font.fontsize()*1.5f}:
                    ((!xticks(ax).empty())?
                        std::array<float,4>{0,0,0,3}:
                        std::array<float,4>{0,0,0,0});
        else return std::array<float,4>{0,0,0,0};
    }
    
    std::array<float,4> ymargin(const std::array<float,4>& ax) const {
        return yticks_margin(ax) +
            (ylabel().empty()?std::array<float,4>{0,0,0,0}:std::array<float,4>{ylabel_font.fontsize()*2,0,0,0});
    }
	
    std::array<float,4> xmargin(const std::array<float,4>& ax) const {
        return xticks_margin(ax) +
            (xlabel().empty()?std::array<float,4>{0,0,0,0}:std::array<float,4>{0,0,0,xlabel_font.fontsize()*2});
    }
    
    std::array<float,4> subplots_margin() const {
        if (subplots_.empty()) return std::array<float,4>{0,0,0,0};
        else {
            float w, h;
            if (figsize_set) { w = this->figsize_[0]; h = this->figsize_[1]; }
            else { auto [x,y] = subplot_size(); w=x; h=y; }
            return std::array<float,4>{w*subplots_adjust_.left(),w*(1.0f-subplots_adjust_.right()),h*(1.0f-subplots_adjust_.top()),h*subplots_adjust_.bottom()};
        }
    }
    
    std::array<float,4> title_margin() const {
        return (title().empty()?std::array<float,4>{1,1,1,1}:std::array<float,4>{1,1,1+title_font.fontsize()*1.5f,1});
    }  
    
    std::array<float,4> margin(const std::array<float,4>& ax) const {
        return xmargin(ax) + ymargin(ax) + subplots_margin() + title_margin();
    }
       

public:    
    template<typename Scale>
    Scale& set_xscale(const Scale& scale,
		typename std::enable_if<std::is_base_of<axis_scale::Base,Scale>::value, int>::type = 0) {
        xscale_ =std::make_unique<Scale>(scale);
        return static_cast<Scale&>(*xscale_);
    }

    template<typename Scale>
    Scale& set_yscale(const Scale& scale,
		typename std::enable_if<std::is_base_of<axis_scale::Base,Scale>::value, int>::type = 0) {
        yscale_ =std::make_unique<Scale>(scale);
        return static_cast<Scale&>(*yscale_);
    } 

    axis_scale::Base& set_xscale(std::string_view s) {
        if (s == "log") return set_xscale(axis_scale::log());
        else if (s == "symlog") return set_xscale(axis_scale::symlog());
        else return set_xscale(axis_scale::linear());
    }

    axis_scale::Base& set_yscale(std::string_view s) {
        if (s == "log") return set_yscale(axis_scale::log());
        else if (s == "symlog") return set_yscale(axis_scale::symlog());
        else return set_yscale(axis_scale::linear());
    } 
    
    template<typename Scale>
    SVGPlot& xscale(const Scale& scale) {
        set_xscale(scale); return (*this);
    }
    

    template<typename Scale>
    SVGPlot& yscale(const Scale& scale) {
        set_yscale(scale); return (*this);
    }

    const axis_scale::Base& xscale() const { return *xscale_; }
    const axis_scale::Base& yscale() const { return *yscale_; }

    SubplotsAdjust& subplots_adjust() { return subplots_adjust_; }
    
	std::array<float,2> figsize() const { 
        if (figsize_set) return figsize_;
        else if ((nrows > 0) && (ncols > 0)) {
            auto marg = subplots_margin()+title_margin();
            auto [width,height] = subplot_size();
            return std::array<float,2>{marg[0]+marg[1]+width,
                                       marg[2]+marg[3]+height};
            
        }
		else return std::array<float,2>{640.0f,480.0f}; // Default size in matplotlib, although instead of inches we have dots and assume 100 dpi
    }
	
	SVGPlot& figsize(const std::array<float,2>& v) { figsize_set=true; figsize_=v; return (*this);}

	std::string_view title() const { return title_; }
	SVGPlot& title(std::string_view l) { title_=l; return (*this); }
	Font& set_title(std::string_view l) { title(l); return title_font; }
	std::string_view ylabel() const { return ylabel_; }
	SVGPlot& ylabel(std::string_view l) { ylabel_=l; return (*this); }
	Font& set_ylabel(std::string_view l) { ylabel(l); return ylabel_font; }
	std::string_view xlabel() const { return xlabel_; }
	SVGPlot& xlabel(std::string_view l) { xlabel_=l; return (*this); }
	Font& set_xlabel(std::string_view l) { xlabel(l); return xlabel_font; }
    
    float linewidth() const { return linewidth_; }
    SVGPlot& linewidth(float f) { linewidth_=f; return *this; }
    void set_linewidth(float f) { linewidth(f); }
	
	SVGPlot& xticks(const std::vector<float>& v) {
		xticks_ = v; xticks_set=true; return (*this);
	}
    template<typename Collection>
    SVGPlot& xticks(const Collection& c) {
        std::vector<float> v;
        for (float f:c) v.push_back(f);
        return xticks(v);
    }
	void set_xticks(const std::vector<float>& v) {
		xticks(v);
	}
	SVGPlot& yticks(const std::vector<float>& v) {
		yticks_ = v; yticks_set=true; return (*this);
	}
    template<typename Collection>
    SVGPlot& yticks(const Collection& c) {
        std::vector<float> v;
        for (float f:c) v.push_back(f);
        return yticks(v);
    }
	void set_yticks(const std::vector<float>& v) {
		yticks(v);
	}
	SVGPlot& xticklabels(const std::vector<std::string>& v) {
		xticklabels_ = v; xticklabels_set=true; return (*this);
	}
    template<typename Collection>
    SVGPlot& xticklabels(const Collection& c) {
        std::vector<std::string> v;
        for (const auto& f:c) v.push_back(f);
        return xticklabels(v);
    }
	void set_xticklabels(const std::vector<std::string>& v) {
		xticklabels(v);
	}
	SVGPlot& yticklabels(const std::vector<std::string>& v) {
		yticklabels_ = v; yticklabels_set=true; return (*this);
	}
    template<typename Collection>
    SVGPlot& yticklabels(const Collection& c) {
        std::vector<std::string> v;
        for (const auto& f:c) v.push_back(f);
        return yticklabels(v);
    }	
    void set_yticklabels(const std::vector<std::string>& v) {
		yticklabels(v);
	}
    template<typename C1, typename C2>
    SVGPlot& xticks(const C1& c1, const C2& c2) {
        return xticks(c1).xticklabels(c2);
    }	    
    template<typename C1>
    SVGPlot& xticks(const C1& c1, const std::vector<std::string>& c2) {
        return xticks(c1).xticklabels(c2);
    }	    
    template<typename C2>
    SVGPlot& xticks(const std::vector<float>& c1, const C2& c2) {
        return xticks(c1).xticklabels(c2);
    }	    
    SVGPlot& xticks(const std::vector<float>& c1, const std::vector<std::string>& c2) {
        return xticks(c1).xticklabels(c2);
    }	    
    template<typename C1, typename C2>
    SVGPlot& yticks(const C1& c1, const C2& c2) {
        return yticks(c1).yticklabels(c2);
    }	    
    template<typename C1>
    SVGPlot& yticks(const C1& c1, const std::vector<std::string>& c2) {
        return yticks(c1).yticklabels(c2);
    }	    
    template<typename C2>
    SVGPlot& yticks(const std::vector<float>& c1, const C2& c2) {
        return yticks(c1).yticklabels(c2);
    }	    
    SVGPlot& yticks(const std::vector<float>& c1, const std::vector<std::string>& c2) {
        return yticks(c1).yticklabels(c2);
    }	    
      
public:
	SVGPlot& axis(const std::array<float,4>& a) { axis_set=true; axis_=a; return (*this); }
	std::array<float,4> axis() const {
		if (axis_set) return axis_;
		else if (plottables.empty()) return std::array<float,4>{0,0,0,0};
        else {
            std::array<float,4> a = plottables.front()->axis();
            auto it = plottables.begin(); ++it;
            for (;it!=plottables.end();++it) a = axis_join(a,(*it)->axis());
            return a;
		}
	}
    
private:	 
	std::array<float,4> scaled_axis() const {
		if (axis_set) {
            auto ax = axis_;
            return std::array<float,4>{xscale().transform(ax[0]),xscale().transform(ax[1]),
                yscale().transform(ax[2]),yscale().transform(ax[3])};
        }
		else if (plottables.empty()) return std::array<float,4>{0,0,0,0};
        else {
            std::array<float,4> a = plottables.front()->scaled_axis(xscale(),yscale());
            auto it = plottables.begin(); ++it;
            for (;it!=plottables.end();++it) a = axis_join(a,(*it)->scaled_axis(xscale(),yscale()));
            return a;
		}
	}

	std::vector<float> xticks(const std::array<float,4>& ax) const {
		if (xticks_set) return xticks_;
		else if (xticklabels_set) {
			auto [xmin,xmax,d1,d2] = ax;
			std::vector<float> sol(xticklabels_.size());
			float dx = (xmax-xmin)/xticklabels_.size();
			for (std::size_t i = 0;i<xticklabels_.size();++i)
				sol[i]=xmin + float(0.5+i)*dx;
			return sol;
		} else {
			auto [xmin,xmax,d1,d2] = ax;
            return xscale().ticks(target_xticks,xscale().antitransform(xmin),xscale().antitransform(xmax));
		}
	}
    
public:   
    std::vector<float> xticks() const { return xticks(scaled_axis()); }

private:	
	std::vector<float> yticks(const std::array<float,4>& ax) const {
		if (yticks_set) return yticks_;
		else if (yticklabels_set) {
			auto [d1,d2,ymin,ymax] = ax;
			std::vector<float> sol(yticklabels_.size());
			float dy = (ymax-ymin)/yticklabels_.size();
			for (std::size_t i = 0;i<yticklabels_.size();++i)
				sol[i]=ymin + float(0.5+i)*dy;
			return sol;
		} else {
			auto [d1,d2,ymin,ymax] = ax;
            return yscale().ticks(target_yticks,yscale().antitransform(ymin),yscale().antitransform(ymax));
		}
	}
public:
    std::vector<float> yticks() const { return yticks(scaled_axis()); }
	
private:
	std::vector<std::string> xticklabels(const std::array<float,4>& ax) const {
		if (xticklabels_set) return xticklabels_;
		else {
			std::vector<float> x = xticks(ax);
			std::vector<std::string> sol(x.size());
			for (std::size_t i = 0; i<x.size(); ++i) sol[i] = xscale().ticklabel(x[i]);
			return sol;
		}
	}
public:
    std::vector<std::string> xticklabels() const { return xticklabels(scaled_axis()); }
    
private:	
	std::vector<std::string> yticklabels(const std::array<float,4>& ax) const {
		if (yticklabels_set) return yticklabels_;
		else {
			std::vector<float> y = yticks(ax);
			std::vector<std::string> sol(y.size());
			for (std::size_t i = 0; i<y.size(); ++i) sol[i] = yscale().ticklabel(y[i]);
			return sol;
		}
	}

public:
    std::vector<std::string> yticklabels() const { return yticklabels(scaled_axis()); }
	

private:
	void add_title(_2d::Group& graph, const std::array<float,2> graph_size, const std::array<float,4>& local_axis) const {
		if (!title().empty()) {
            auto marg = margin(local_axis);
			graph.add(
				_2d::text({0.5*(graph_size[0]-marg[0]-marg[1]),-0.5*title_font.fontsize()},title()))
					.font_size(title_font.fontsize())
					.text_anchor(svg_cpp_plot::text_anchor_middle);
		}
	}

	void add_xlabel(_2d::Group& graph, const std::array<float,2> graph_size, const std::array<float,4>& local_axis) const {
		if (!xlabel().empty()) {
            auto marg = margin(local_axis);
			graph.add(
				_2d::text({0.5*(graph_size[0]-marg[0]-marg[1]),graph_size[1]+10+xlabel_font.fontsize()-marg[2]-marg[3]},xlabel()))
					.font_size(xlabel_font.fontsize())
					.text_anchor(svg_cpp_plot::text_anchor_middle);
		}
	}
	
	void add_ylabel(_2d::Group& graph, const std::array<float,2> graph_size, const std::array<float,4>& local_axis) const {
		if (!ylabel().empty()) {
            auto marg = margin(local_axis);
			graph.add(_2d::group(_2d::translate({-10-ylabel_font.fontsize(),0.5*(graph_size[1]-marg[2]-marg[3])})*_2d::rotate(-0.5*M_PI))).add(_2d::text({0,0},ylabel())).font_size(ylabel_font.fontsize()).text_anchor(text_anchor_middle);
		}
	}
	
	void add_xticks(_2d::Group& graph, const std::array<float,2> graph_size, const std::array<float,4>& local_axis) const {
        auto marg = margin(local_axis);
		auto x = xticks(local_axis);
		auto labels = xticklabels(local_axis);
		for (std::size_t i=0;i<x.size();++i) {
			float global_x = (graph_size[0] - (marg[0]+marg[1]))*(xscale().transform(x[i]) - local_axis[0])/
						(local_axis[1]-local_axis[0]);
			graph.add(_2d::line(
				{global_x,graph_size[1]-(marg[2]+marg[3])},
				{global_x,graph_size[1]-(marg[2]+marg[3])+3}))
				.stroke(black).stroke_width(linewidth());
			if (i<labels.size()) {
				graph.add(
					_2d::text({global_x,graph_size[1]-(marg[2]+marg[3])+5},labels[i]))
						.font_size(xticklabels_font.fontsize())
						.text_anchor(text_anchor_middle)
						.dominant_baseline(dominant_baseline_hanging);
			}
		}
	}
	
	void add_yticks(_2d::Group& graph, const std::array<float,2> graph_size, const std::array<float,4>& local_axis) const {
        auto marg = margin(local_axis);
		auto y = yticks(local_axis);
		auto labels = yticklabels(local_axis);
		for (std::size_t i=0;i<y.size();++i) {
			float global_y = graph_size[1] - (marg[2]+marg[3]) - (graph_size[1]-marg[2]-marg[3])*(yscale().transform(y[i]) - local_axis[2])/(local_axis[3]-local_axis[2]);
			graph.add(_2d::line({-3,global_y},{0,global_y}))
				.stroke(black).stroke_width(linewidth());
			if (i<labels.size()) {		
				graph.add(_2d::text({-5,global_y},labels[i])).font_size(yticklabels_font.fontsize()).text_anchor(text_anchor_end).dominant_baseline(dominant_baseline_middle);
			}
		}
	}

public: 
	_2d::Group graph() const {
		std::array<float,2> graph_size = figsize();
		std::array<float,4> local_axis = scaled_axis();
		auto marg = margin(local_axis);
       
        auto group = _2d::group(_2d::translate({marg[0],marg[2]}));
		
        if ((ncols>0) && (nrows>0)) {  
            float ypos = 0;   
            for (int r = 0;r<nrows;++r) {
                float xpos = 0;
                for (int c = 0;c<ncols;++c)
                    if (subplots_[r*ncols+c]) {
                        auto subplot = _2d::group(_2d::translate({xpos,ypos}));
                        subplot.add(subplots_[r*ncols+c]->graph());
                        group.add(subplot);
                        xpos += subplots_xmax(c)+(graph_size[0]-marg[0]-marg[1])*subplots_adjust_.wspace()/float(ncols);
                    }
                ypos+=subplots_ymax(r)+(graph_size[1]-marg[2]-marg[3])*subplots_adjust_.hspace()/float(nrows);    
            }
        } else {
            std::tuple<float,float> area_size(graph_size[0] - (marg[1]+marg[0]),graph_size[1] - (marg[2]+marg[3]));
            group.add(_2d::clip_path()).id(group.id()+"clipper").add(_2d::rect({0,0},area_size));
            auto& area = group.add(_2d::group(
                _2d::scale(std::get<0>(area_size)/(local_axis[1]-local_axis[0]),-std::get<1>(area_size)/(local_axis[3]-local_axis[2]))*
                _2d::translate(-local_axis[0],-local_axis[3])));
            area.set("clip-path",std::string("url(#")+group.id()+"clipper)");
            group.add(_2d::rect({0,0},area_size)).fill(none).stroke_width(linewidth()).stroke(black).pointer_events(pointer_events_none);
            for (const auto& p : plottables) area.add_ptr(p->scaled(xscale(),yscale()));
		
            add_xticks(group, graph_size, local_axis);
            add_yticks(group, graph_size, local_axis);
        }
        add_xlabel(group, graph_size, local_axis);
        add_ylabel(group, graph_size, local_axis);
        add_title(group, graph_size, local_axis); 
		return group;
	}

protected:		
	SVG svg() const {
		std::array<float,2> graph_size = figsize();
		SVG s;
		s.add(graph());
		s.viewBox(BoundingBox(0,0,graph_size[0],graph_size[1]));
		return s;
	}
public:
	SVGPlot() :
		figsize_set(false), cycle(detail::color_vector({"#1f77b4","#ff7f0e","#2ca02c","#d62728","#9467bd","#8c564b","#e377c2","#7f7f7f","#bcbd22","#17becf"})), cycle_pos(0), ylabel_font(18), xlabel_font(18), title_font(24), xticklabels_font(14), yticklabels_font(14), axis_set(false),xticklabels_set(false), xticks_set(false), yticklabels_set(false), yticks_set(false), linewidth_(1), 
        xscale_(std::make_unique<axis_scale::linear>()), yscale_(std::make_unique<axis_scale::linear>()), nrows(-1), ncols(-1), parent(nullptr), target_xticks(5), target_yticks(5) {	}
        
    /*******************************************************
     * SUBPLOT
     *******************************************************/
    SVGPlot& subplot(int nrows_, int ncols_, std::size_t index) {
        if ((nrows_!=nrows) && (ncols_!=ncols)) {
            nrows=nrows_; 
            ncols=ncols_;
            subplots_.resize(nrows*ncols);
        }
        if (!subplots_[index]) {
            subplots_[index] = std::make_unique<SVGPlot>();
            subplots_[index]->parent = this;
        }
        return *subplots_[index];
    }
    
    /*******************************************************
     * IMSHOW VERSIONS
     *******************************************************/
	
    protected:
    void imshow_autoticks(const ImShow& imshow) {
        auto [width,height] = imshow.size();
        if (width < target_xticks) { target_xticks = width - 1; }
        if (height < target_yticks) { target_yticks = height - 1; }
    }
    
    public:
	ImShow& imshow(const std::vector<std::vector<float>>& data) {      
		plottables.push_back(std::make_shared<ImShowType<float>>(data));
        ImShow& is =  static_cast<ImShow&>(*plottables.back());
        imshow_autoticks(is);
		return is;
	}
    
    ImShow& imshow(const std::vector<std::vector<std::tuple<float,float,float>>>& data) {
		plottables.push_back(std::make_shared<ImShowType<std::tuple<float,float,float>>>(data));
        ImShow& is =  static_cast<ImShow&>(*plottables.back());
        imshow_autoticks(is);
		return is;
	}
    
    ImShow& imshow(const std::vector<std::vector<std::tuple<float,float,float,float>>>& data) {
		plottables.push_back(std::make_shared<ImShowType<std::tuple<float,float,float,float>>>(data));
        ImShow& is =  static_cast<ImShow&>(*plottables.back());
        imshow_autoticks(is);
		return is;
	}
private:
    template<typename V>
    float imshow_convert(const V& v) const {
        return float(v);
    }
    
    template<typename V1,typename V2, typename V3>
    std::tuple<float,float,float> imshow_convert(const std::tuple<V1,V2,V3>& v) const {
        return std::tuple<float,float,float>(imshow_convert(std::get<0>(v)),imshow_convert(std::get<1>(v)),imshow_convert(std::get<2>(v)));
    }
    
    template<typename V1,typename V2, typename V3, typename V4>
    std::tuple<float,float,float,float> imshow_convert(const std::tuple<V1,V2,V3,V4>& v) const {
        return std::tuple<float,float,float,float>(imshow_convert(std::get<0>(v)),imshow_convert(std::get<1>(v)),
                    imshow_convert(std::get<2>(v)),imshow_convert(std::get<3>(v)));
    }
   
    template<typename V>
    std::tuple<float,float,float> imshow_convert(const std::array<V,3>& a) const {
        return std::tuple<float,float,float>(imshow_convert(a[0]),imshow_convert(a[1]),imshow_convert(a[2]));
    }
    
    template<typename V>
    std::tuple<float,float,float,float> imshow_convert(const std::array<V,4>& a) const {
        return std::tuple<float,float,float,float>(imshow_convert(a[0]),imshow_convert(a[1]),imshow_convert(a[2]),imshow_convert(a[3]));
    }

public:    
    template<typename Collection>
    ImShow& imshow(const Collection& data) {
        using T = typename std::decay<decltype(imshow_convert(data.front().front()))>::type;
        std::vector<std::vector<T>> d;
        for (const auto& row : data) {
            d.push_back(std::vector<T>());
            for (const auto& f : row) d.back().push_back(imshow_convert(f));
        }
        return imshow(d);
    }
    
    template<typename XRange, typename YRange, typename F>
    ImShow& imshow(const XRange& xrange, const YRange& yrange, const F& f) {
        if (!xticks_set) xticks({xrange.front(),xrange.back()});
        if (!yticks_set) yticks({yrange.front(),yrange.back()});
        using T = typename std::decay<decltype(imshow_convert(f(xrange.front(),yrange.front())))>::type;
        std::vector<std::vector<T>> data;
        for (auto y : yrange) {  
            data.push_back(std::vector<T>());
            for (auto x : xrange) data.back().push_back(f(x,y)); 
        }
        ImShow& is = imshow(data);
        auto [width,height] = is.size();
        float dx = (xrange.back()-xrange.front())/float(width);
        float dy = (yrange.back()-yrange.front())/float(height);
        return is.extent({xrange.front()-0.5f*dx,xrange.back()+0.5f*dx,yrange.front()-0.5f*dy,yrange.back()+0.5f*dy});
	}
    
    /*******************************************************
     * SCATTER VERSIONS
     *******************************************************/
    template<typename X, typename Y>
	Scatter& scatter(const X& x, const Y& y,
		typename std::enable_if<std::is_floating_point<typename X::value_type>::value && std::is_floating_point<typename Y::value_type>::value, int>::type = 0) {
        
        plottables.push_back(std::make_shared<Scatter>(x,y));
        auto color = cycle[cycle_pos];
		cycle_pos = (cycle_pos + 1) % cycle.size();
        return static_cast<Scatter&>(*plottables.back()).c(color);
    }
    
    template<typename X>
	Scatter&  scatter(const X& x, const std::initializer_list<float>& y) {
		return scatter(x,std::list<float>(y));
	}
	
	template<typename Y>
	Scatter&  scatter(const std::initializer_list<float>& x,
			  const Y& y) {
		return scatter(std::list<float>(x),y);
	}
	
	Scatter&  scatter(const std::initializer_list<float>& x,
			  const std::initializer_list<float>& y) {	  
		return scatter(std::list<float>(x),std::list<float>(y));
	}

    /*******************************************************
     * PLOT VERSIONS
     *******************************************************/
		
	template<typename X, typename Y>
	Plot& plot(const X& x, const Y& y, std::string_view fmt = "",
		typename std::enable_if<std::is_floating_point<typename X::value_type>::value && std::is_floating_point<typename Y::value_type>::value, int>::type = 0) {
			std::shared_ptr<Color> color = nullptr;
			std::string_view dashes = fmt;
			if (fmt.size()>0) {
				dashes = fmt.substr(1);
				switch (fmt[0]) {
					case 'r': color = std::make_shared<redColor>();     break;
					case 'g': color = std::make_shared<greenColor>();   break;
					case 'b': color = std::make_shared<blueColor>();    break;
					case 'k': color = std::make_shared<blackColor>();   break;
					case 'w': color = std::make_shared<whiteColor>();   break;
					case 'c': color = std::make_shared<cyanColor>();    break;
					case 'm': color = std::make_shared<magentaColor>(); break;
					case 'y': color = std::make_shared<yellowColor>();  break;
					default: dashes = fmt;
				}
			}
			if (!color) {
				color = cycle[cycle_pos];
				cycle_pos = (cycle_pos + 1) % cycle.size();
			}
            plottables.push_back(std::make_shared<Plot>(x,y));
            return static_cast<Plot&>(*plottables.back()).color(color).format(dashes);
	}
	
	template<typename X, typename Y>
	Plot&  plot(const X& x, const Y& y, std::string_view fmt = "",
		typename std::enable_if<std::is_floating_point<typename X::value_type>::value && std::is_floating_point<decltype(std::declval<Y>()(0.0f))>::value, int>::type = 0) {
		
		std::list<float> l;
		for (float ix : x) l.push_back(y(ix));
		return plot(x,l,fmt);
	}
	
	template<typename X>
	Plot&  plot(const X& x, const std::initializer_list<float>& y, std::string_view fmt = "") {
		return plot(x,std::list<float>(y), fmt);
	}
	
	template<typename Y>
	Plot&  plot(const std::initializer_list<float>& x,
			  const Y& y, std::string_view fmt = "") {
		return plot(std::list<float>(x),y, fmt);
	}
	
	Plot&  plot(const std::initializer_list<float>& x,
			  const std::initializer_list<float>& y, std::string_view fmt = "") {	  
		return plot(std::list<float>(x),std::list<float>(y),fmt);
	}
	
	template<typename Y>
	Plot&  plot(const Y& y, std::string_view fmt  = "") {	  
		return plot(arange(0,y.size(),1),y,fmt);
	}
	
	Plot&  plot(const std::initializer_list<float>& y, std::string_view fmt  = "") {	  
		return plot(arange(0,y.size(),1),std::list<float>(y),fmt);
	}

    /*****************************************************
     * BAR
     *****************************************************/

    Bar& bar(const std::vector<float>& x, const std::vector<float>& height) {
		auto color = cycle[cycle_pos];
		cycle_pos = (cycle_pos + 1) % cycle.size();
        plottables.push_back(std::make_shared<Bar>(x,height));
        return static_cast<Bar&>(*plottables.back()).color(color);
    }
    
    Bar& bar(const std::vector<std::string>& x, const std::vector<float>& height) {
        this->xticks(arange(x.size()),x);
        return this->bar(arange(x.size()),height);
    }
    
    template<typename C1>
    Bar& bar(const C1& x, const std::vector<float>& height) {
        if constexpr (std::is_arithmetic<typename std::decay<typename C1::value_type>::type>::value)
            return bar(std::vector<float>(x.begin(),x.end()),height);
        else
            return bar(std::vector<std::string>(x.begin(),x.end()),height);
    }
    
    template<typename C2>
    Bar& bar(const std::vector<float>& x, const C2& height) {
        return bar(x,std::vector<float>(height.begin(),height.end()));
    }
	
    template<typename C1,typename C2>
    Bar& bar(const C1& x, const C2& height) {
        if constexpr (std::is_arithmetic<typename std::decay<typename C1::value_type>::type>::value)
            return bar(std::vector<float>(x.begin(),x.end()),std::vector<float>(height.begin(),height.end()));
        else
            return bar(std::vector<std::string>(x.begin(),x.end()),std::vector<float>(height.begin(),height.end()));
    }
    
    /*****************************************************
     * BARH
     *****************************************************/
    
    BarH& barh(const std::vector<float>& y, const std::vector<float>& width) {
		auto color = cycle[cycle_pos];
		cycle_pos = (cycle_pos + 1) % cycle.size();
        plottables.push_back(std::make_shared<BarH>(y,width));
        return static_cast<BarH&>(*plottables.back()).color(color);
    }
    
    BarH& barh(const std::vector<std::string>& y, const std::vector<float>& height) {
        this->yticks(arange(y.size()),y);
        return this->barh(arange(y.size()),height);
    }
    
    template<typename C1>
    BarH& barh(const C1& y, const std::vector<float>& width) {
        if constexpr (std::is_arithmetic<typename std::decay<typename C1::value_type>::type>::value)
            return barh(std::vector<float>(y.begin(),y.end()),width);
        else
            return barh(std::vector<std::string>(y.begin(),y.end()),width);
    }
    
    template<typename C2>
    BarH& barh(const std::vector<float>& y, const C2& width) {
        return barh(y,std::vector<float>(width.begin(),width.end()));
    }
	
    template<typename C1,typename C2>
    BarH& barh(const C1& y, const C2& width) {
        if constexpr (std::is_arithmetic<typename std::decay<typename C1::value_type>::type>::value)
            return barh(std::vector<float>(y.begin(),y.end()),std::vector<float>(width.begin(),width.end()));
        else
            return barh(std::vector<std::string>(y.begin(),y.end()),std::vector<float>(width.begin(),width.end()));
    }
    
    /*****************************************************
     * HIST
     *****************************************************/
    Hist& hist(const std::vector<float>& x) {
		auto color = cycle[cycle_pos];
		cycle_pos = (cycle_pos + 1) % cycle.size();
        plottables.push_back(std::make_shared<Hist>(x));
        return static_cast<Hist&>(*plottables.back()).color(color);
    } 
    
    template<typename Collection>
    Hist& hist(const Collection& x) {
        return hist(std::vector<float>(x.begin(),x.end()));
    } 
    
	void savefig(const std::string& name) const {
		std::ofstream f(name);
		f<<svg();
	}	
};
}
