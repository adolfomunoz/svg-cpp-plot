#pragma once

#include <list>
#include <tuple>
#include <memory>
#include "svgplot/arange.h"
#include "graph-2d.h"
#include "svgplot/plot.h"
#include "svgplot/imshow.h"
#include "svgplot/bar.h"
#include "../2d/point-list.h"
#include "../2d/polyline.h"

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


		
class SVGPlot {
	std::array<float,2> figsize_; bool figsize_set;
	std::vector<std::shared_ptr<Color>> cycle; std::size_t cycle_pos;

	std::string ylabel_, xlabel_, title_;
	std::array<float,4> axis_; bool axis_set;
	
	std::vector<std::string> xticklabels_; bool xticklabels_set;
	std::vector<float> xticks_; bool xticks_set;
	std::vector<std::string> yticklabels_; bool yticklabels_set;
	std::vector<float> yticks_; bool yticks_set;
	
	
    std::list<std::shared_ptr<Plottable>> plottables;
	
	
	std::vector<std::unique_ptr<SVGPlot>> subplots_;
    int nrows, ncols;
    class SubplotsAdjust {
        float left_, right_, bottom_, top_, wspace_, hspace_;
    public:
        SubplotsAdjust() : left_(0), right_(0), bottom_(0), top_(0), wspace_(0), hspace_(0) {}
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

    std::array<float,4> yticks_margin() const {
        if (subplots_.empty())
            return (!yticklabels().empty())?
                    std::array<float,4>{25,0,0,0}:
                    ((!yticks().empty())?
                        std::array<float,4>{3,0,0,0}:
                        std::array<float,4>{0,0,0,0});
        else return std::array<float,4>{0,0,0,0};
    }
    
    std::array<float,4> xticks_margin() const {
        if (subplots_.empty())
            return (!xticklabels().empty())?
                    std::array<float,4>{0,0,0,15}:
                    ((!xticks().empty())?
                        std::array<float,4>{0,0,0,3}:
                        std::array<float,4>{0,0,0,0});
        else return std::array<float,4>{0,0,0,0};
    }
    
    std::array<float,4> ymargin() const {
        return yticks_margin() +
            (ylabel().empty()?std::array<float,4>{0,0,0,0}:std::array<float,4>{40,0,0,0});
    }
	
    std::array<float,4> xmargin() const {
        return xticks_margin() +
            (xlabel().empty()?std::array<float,4>{0,0,0,0}:std::array<float,4>{0,0,0,30});
    }
    
    std::array<float,4> subplots_margin() const {
         if (subplots_.empty()) return std::array<float,4>{0,0,0,0};
         else return std::array<float,4>{subplots_adjust_.left(),subplots_adjust_.right(),subplots_adjust_.top(),subplots_adjust_.bottom()};
    }
    
    std::array<float,4> margin() const {
        return xmargin() + ymargin() + subplots_margin() + 
            (title().empty()?std::array<float,4>{5,5,5,5}:std::array<float,4>{5,5,35,5});
    }
    
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
 
    
public:
    SubplotsAdjust& subplots_adjust() { return subplots_adjust_; }
    
	std::array<float,2> figsize() const { 
        if (figsize_set) return figsize_;
        else if ((nrows > 0) && (ncols > 0)) {
            auto marg = margin();
            float width = 0, height = 0;
            for (int r = 0; r<nrows; ++r) height += subplots_ymax(r);
            for (int c = 0; c<ncols; ++c) width += subplots_xmax(c);
            return std::array<float,2>{width+marg[0]+marg[1]+ncols*subplots_adjust_.wspace(),
                                       height+marg[2]+marg[3]+nrows*subplots_adjust_.hspace()};
            
        }
		else return std::array<float,2>{200.0f,150.0f}; 
    }
	
	SVGPlot& figsize(const std::array<float,2>& v) { figsize_set=true; figsize_=v; return (*this);}

	std::string_view title() const { return title_; }
	SVGPlot& title(std::string_view l) { title_=l; return (*this); }
	void set_title(std::string_view l) { title(l); }
	std::string_view ylabel() const { return ylabel_; }
	SVGPlot& ylabel(std::string_view l) { ylabel_=l; return (*this); }
	void set_ylabel(std::string_view l) { ylabel(l); }
	std::string_view xlabel() const { return xlabel_; }
	SVGPlot& xlabel(std::string_view l) { xlabel_=l; return (*this); }
	void set_xlabel(std::string_view l) { xlabel(l); }
	
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
private:
    static void axis_include(std::array<float,4>& a, const std::array<float,4>& b) {
        if (a[0]>b[0]) a[0]=b[0];
        if (a[1]<b[1]) a[1]=b[1];
        if (a[2]>b[2]) a[2]=b[2];
        if (a[3]<b[3]) a[3]=b[3];
    }        
public:
	SVGPlot& axis(const std::array<float,4>& a) { axis_set=true; axis_=a; return (*this); }
	std::array<float,4> axis() const {
		if (axis_set) return axis_;
		else if (plottables.empty()) return std::array<float,4>{0,0,0,0};
        else {
            std::array<float,4> a = plottables.front()->axis();
            for (const auto& plottable : plottables) axis_include(a,plottable->axis());
            return a;
		}
	}
	
	std::vector<float> xticks() const {
		if (xticks_set) return xticks_;
		else if (xticklabels_set) {
			auto [xmin,xmax,d1,d2] = axis();
			std::vector<float> sol(xticklabels_.size());
			float dx = (xmax-xmin)/xticklabels_.size();
			for (std::size_t i = 0;i<xticklabels_.size();++i)
				sol[i]=xmin + float(0.5+i)*dx;
			return sol;
		} else {
			const float target_ticks = float(target_xticks);
			auto [xmin,xmax,d1,d2] = axis();
			float tick_step = std::floor((xmax - xmin)/float(target_ticks));
			int factor = 2;
			while (tick_step<=0.0f) {
				tick_step=std::floor(factor*(xmax - xmin)/float(target_ticks))/float(factor);
				if ((factor % 4) == 0) factor = (factor*10)/4;
				else factor*=2;
			}
			float first_tick = std::ceil(xmin/tick_step)*tick_step;
			std::vector<float> sol;
			for (float x = first_tick; x <= xmax; x+=tick_step)
				sol.push_back(x);
			return sol;
		}
	}
	
	std::vector<float> yticks() const {
		if (yticks_set) return yticks_;
		else if (yticklabels_set) {
			auto [d1,d2,ymin,ymax] = axis();
			std::vector<float> sol(yticklabels_.size());
			float dy = (ymax-ymin)/yticklabels_.size();
			for (std::size_t i = 0;i<yticklabels_.size();++i)
				sol[i]=ymin + float(0.5+i)*dy;
			return sol;
		} else {
			const float target_ticks = float(target_yticks);
			auto [d1,d2,ymin,ymax] = axis();
			float tick_step = std::floor((ymax - ymin)/float(target_ticks));
			int factor = 2;
			while (tick_step<=0.0f) {
				tick_step=std::floor(factor*(ymax - ymin)/float(target_ticks))/float(factor);
				if ((factor % 4) == 0) factor = (factor*10)/4;
				else factor*=2;
			}
			float first_tick = std::ceil(ymin/tick_step)*tick_step;
			std::vector<float> sol;
			for (float y = first_tick; y <= ymax; y+=tick_step)
				sol.push_back(y);
			return sol;
		}
	}
	
	std::vector<std::string> xticklabels() const {
		if (xticklabels_set) return xticklabels_;
		else {
			std::vector<float> x = xticks();
			std::vector<std::string> sol(x.size());
			for (std::size_t i = 0; i<x.size(); ++i) {
				std::stringstream s; 
				s<<((x[i]==0)?0:x[i]);
				sol[i]=s.str();
			}
			return sol;
		}
	}
	
	std::vector<std::string> yticklabels() const {
		if (yticklabels_set) return yticklabels_;
		else {
			std::vector<float> y = yticks();
			std::vector<std::string> sol(y.size());
			for (std::size_t i = 0; i<y.size(); ++i) {
				std::stringstream s; 
				s<<((y[i]==0)?0:y[i]);
				sol[i]=s.str();
			}
			return sol;
		}
	}
	

private:
	void add_title(_2d::Group& graph, const std::array<float,2> graph_size) const {
		if (!title().empty()) {
            auto marg = margin();
			graph.add(
				_2d::text({0.5*(graph_size[0]-marg[0]-marg[1]),-10},title()))
					.font_size(16)
					.text_anchor(svg_cpp_plot::text_anchor_middle);
		}
	}

	void add_xlabel(_2d::Group& graph, const std::array<float,2> graph_size) const {
		if (!xlabel().empty()) {
            auto marg = margin();
			graph.add(
				_2d::text({0.5*(graph_size[0]-marg[0]-marg[1]),graph_size[1]+26-marg[2]-marg[3]},xlabel()))
					.font_size(14)
					.text_anchor(svg_cpp_plot::text_anchor_middle);
		}
	}
	
	void add_ylabel(_2d::Group& graph, const std::array<float,2> graph_size) const {
		if (!ylabel().empty()) {
            auto marg = margin();
			graph.add(_2d::group(_2d::translate({-26,0.5*(graph_size[1]-marg[2]-marg[3])})*_2d::rotate(-0.5*M_PI))).add(_2d::text({0,0},ylabel())).font_size(14).text_anchor(text_anchor_middle);
		}
	}
	
	void add_xticks(Graph2D& graph, const std::array<float,2> graph_size, const std::array<float,4>& local_axis) const {
        auto marg = margin();
		auto x = xticks();
		auto labels = xticklabels();
		for (std::size_t i=0;i<x.size();++i) {
			float global_x = (graph_size[0] - (marg[0]+marg[1]))*(x[i] - local_axis[0])/
						(local_axis[1]-local_axis[0]);
			graph.add(_2d::line(
				{global_x,graph_size[1]-(marg[2]+marg[3])},
				{global_x,graph_size[1]-(marg[2]+marg[3])+3}))
				.stroke(black).stroke_width(1);
			if (i<labels.size()) {
				graph.add(
					_2d::text({global_x,graph_size[1]-(marg[2]+marg[3])+5},labels[i]))
						.font_size(10)
						.text_anchor(text_anchor_middle)
						.dominant_baseline(dominant_baseline_hanging);
			}
		}
	}
	
	void add_yticks(Graph2D& graph, const std::array<float,2> graph_size, const std::array<float,4>& local_axis) const {
        auto marg = margin();
		auto y = yticks();
		auto labels = yticklabels();
		for (std::size_t i=0;i<y.size();++i) {
			float global_y = graph_size[1] - (marg[2]+marg[3]) - (graph_size[1]-marg[2]-marg[3])*(y[i] - local_axis[2])/(local_axis[3]-local_axis[2]);
			graph.add(_2d::line({-3,global_y},{0,global_y}))
				.stroke(black).stroke_width(1);
			if (i<labels.size()) {		
				graph.add(_2d::text({-5,global_y},labels[i])).font_size(10).text_anchor(text_anchor_end).dominant_baseline(dominant_baseline_middle);
			}
		}
	}

public: 
	_2d::Group graph() const {
		std::array<float,2> graph_size = figsize();
		auto marg = margin();
		std::array<float,4> local_axis = axis();
       
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
                        xpos += subplots_xmax(c)+subplots_adjust_.wspace();
                    }
                ypos+=subplots_ymax(r)+subplots_adjust_.hspace();    
            }
        } else {
            Graph2D graph({graph_size[0] - (marg[1]+marg[0]),graph_size[1] -(marg[3]+marg[2])},BoundingBox(local_axis[0],local_axis[2],local_axis[1],local_axis[3]));
            for (const auto& p : plottables) graph.area().add_ptr(p);
		
            add_xticks(graph, graph_size, local_axis);
            add_yticks(graph, graph_size, local_axis);
            graph.border().stroke_width(1).stroke(black);
            group.add(graph);
        }
        add_xlabel(group, graph_size);
        add_ylabel(group, graph_size);
        add_title(group, graph_size); 
		

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
		figsize_set(false), cycle(detail::color_vector({"#1f77b4","#ff7f0e","#2ca02c","#d62728","#9467bd","#8c564b","#e377c2","#7f7f7f","#bcbd22","#17becf"})), cycle_pos(0), axis_set(false),xticklabels_set(false), xticks_set(false), yticklabels_set(false), yticks_set(false), nrows(-1), ncols(-1), parent(nullptr), target_xticks(5), target_yticks(5) {	}
        
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
    
	void savefig(const std::string& name) const {
		std::ofstream f(name);
		f<<svg();
	}	
};

}
