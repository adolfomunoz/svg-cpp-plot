#pragma once

#include <list>
#include <tuple>
#include <memory>
#include "svgplot/arange.h"
#include "graph-2d.h"
#include "svgplot/plot.h"
#include "svgplot/imshow.h"
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
	std::array<float,2> figsize_;
	std::vector<std::unique_ptr<Color>> cycle; std::size_t cycle_pos;

	PlotGroup plots;
	
	std::string ylabel_, xlabel_, title_;
	std::array<float,4> axis_; bool axis_set;
	
	std::vector<std::string> xticklabels_; bool xticklabels_set;
	std::vector<float> xticks_; bool xticks_set;
	std::vector<std::string> yticklabels_; bool yticklabels_set;
	std::vector<float> yticks_; bool yticks_set;
	
	
	std::shared_ptr<ImShow> imshow_;
	
	
	std::vector<std::unique_ptr<SVGPlot>> subplots_;
    int nrows, ncols;
	SVGPlot* parent; 
    
    unsigned long target_xticks, target_yticks;
public:
	std::array<float,2> figsize() const { 
		if (parent) return std::array<float,2>{
			parent->figsize()[0]/float(parent->nrows),
			parent->figsize()[1]/float(parent->ncols)};	
		else return figsize_; }
	
	SVGPlot& figsize(const std::array<float,2>& v) { figsize_=v; return (*this);}

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
	void set_xticks(const std::vector<float>& v) {
		xticks(v);
	}
	SVGPlot& yticks(const std::vector<float>& v) {
		yticks_ = v; yticks_set=true; return (*this);
	}
	void set_yticks(const std::vector<float>& v) {
		yticks(v);
	}
	SVGPlot& xticklabels(const std::vector<std::string>& v) {
		xticklabels_ = v; xticklabels_set=true; return (*this);
	}
	void set_xticklabels(const std::vector<std::string>& v) {
		xticklabels(v);
	}
	SVGPlot& yticklabels(const std::vector<std::string>& v) {
		yticklabels_ = v; yticklabels_set=true; return (*this);
	}
	void set_yticklabels(const std::vector<std::string>& v) {
		yticklabels(v);
	}

	SVGPlot& axis(const std::array<float,4> a) { axis_set=true; axis_=a; return (*this); }
	std::array<float,4> axis() const {
		if (axis_set) return axis_;
		else if (imshow_) return imshow_->axis();
		else {
			float minx(0), miny(0), maxx(0), maxy(0); bool first=true;
			for (const auto& plot : plots)
				for (const auto& [x,y] : plot) {
					if (first || (minx>x)) minx=x;
					if (first || (maxx<x)) maxx=x;
					if (first || (miny>y)) miny=y;
					if (first || (maxy<y)) maxy=y;
					first = false;
				}
		
			float dx = (maxx-minx)/32.0f;
			float dy = (maxy-miny)/32.0f;
			return std::array<float,4>{minx-dx,maxx+dx,miny-dy,maxy+dy};
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

    std::array<float,4> yticks_margin() const {
        return (!yticklabels().empty())?
                    std::array<float,4>{25,0,0,0}:
                    ((!yticks().empty())?
                        std::array<float,4>{3,0,0,0}:
                        std::array<float,4>{0,0,0,0});
    }
    
    std::array<float,4> xticks_margin() const {
        return (!xticklabels().empty())?
                    std::array<float,4>{0,0,0,15}:
                    ((!xticks().empty())?
                        std::array<float,4>{0,0,0,3}:
                        std::array<float,4>{0,0,0,0});
    }
    
    std::array<float,4> ymargin() const {
        return yticks_margin() +
            (ylabel().empty()?std::array<float,4>{0,0,0,0}:std::array<float,4>{40,0,0,0});
    }
	
    std::array<float,4> xmargin() const {
        return xticks_margin() +
            (xlabel().empty()?std::array<float,4>{0,0,0,0}:std::array<float,4>{0,0,0,30});
    }
    
    std::array<float,4> margin() const {
        return xmargin() + ymargin() +
            (title().empty()?std::array<float,4>{5,5,5,5}:std::array<float,4>{5,5,35,5});
    }
	
    
	void add_title(Graph2D& graph, const std::array<float,2> graph_size) const {
		if (!title().empty()) {
			graph.add(
				_2d::text({0.5*graph_size[0],-10},title()))
					.font_size(16)
					.text_anchor(svg_cpp_plot::text_anchor_middle);
		}
	}

	void add_xlabel(Graph2D& graph, const std::array<float,2> graph_size) const {
		if (!xlabel().empty()) {
			graph.add(
				_2d::text({0.5*graph_size[0],graph_size[1]+26},xlabel()))
					.font_size(14)
					.text_anchor(svg_cpp_plot::text_anchor_middle);
		}
	}
	
	void add_ylabel(Graph2D& graph, const std::array<float,2> graph_size) const {
		if (!ylabel().empty()) {
			graph.add(_2d::group(_2d::translate({-26,0.5*graph_size[1]})*_2d::rotate(-0.5*M_PI))).add(_2d::text({0,0},ylabel())).font_size(14).text_anchor(text_anchor_middle);
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
	auto graph() const {
		std::array<float,2> graph_size = figsize();
		auto marg = margin();
		std::array<float,4> local_axis = axis();
		Graph2D graph({graph_size[0] - (marg[1]+marg[0]),graph_size[1] -(marg[3]+marg[2])},BoundingBox(local_axis[0],local_axis[2],local_axis[1],local_axis[3]));
		
		if (imshow_) graph.area().add_ptr(imshow_);
		for (auto p : plots) graph.area().add(p);
		
		add_xticks(graph, graph_size, local_axis);
		add_yticks(graph, graph_size, local_axis);
		add_xlabel(graph, graph_size);
		add_ylabel(graph, graph_size);
		add_title( graph, graph_size);
		
        auto group = _2d::group(_2d::translate({marg[0],marg[2]}));

		graph.border().stroke_width(1).stroke(black);
        group.add(graph);
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
		figsize_{200,150}, cycle_pos(0), axis_set(false),xticklabels_set(false), xticks_set(false), yticklabels_set(false), yticks_set(false), nrows(-1), ncols(-1), parent(nullptr), target_xticks(5), target_yticks(5) {
			cycle.push_back(std::make_unique<color_hex>("1f77b4"));
			cycle.push_back(std::make_unique<color_hex>("ff7f0e"));
			cycle.push_back(std::make_unique<color_hex>("2ca02c"));
			cycle.push_back(std::make_unique<color_hex>("d62728"));
			cycle.push_back(std::make_unique<color_hex>("9467bd"));
			cycle.push_back(std::make_unique<color_hex>("8c564b"));
			cycle.push_back(std::make_unique<color_hex>("e377c2"));
			cycle.push_back(std::make_unique<color_hex>("7f7f7f"));
			cycle.push_back(std::make_unique<color_hex>("bcbd22"));
			cycle.push_back(std::make_unique<color_hex>("17becf"));
		}
        
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
    void imshow_autoticks() {
        auto [width,height] = imshow_->size();
        if (width < target_xticks) { target_xticks = width - 1; }
        if (height < target_yticks) { target_yticks = height - 1; }
    }
    
    public:
	ImShow& imshow(const std::vector<std::vector<float>>& data) {
		imshow_=std::make_shared<ImShowType<float>>(data);
        imshow_autoticks();
		return *imshow_;
	}
    
    ImShow& imshow(const std::vector<std::vector<std::tuple<float,float,float>>>& data) {
		imshow_=std::make_shared<ImShowType<std::tuple<float,float,float>>>(data);
        imshow_autoticks();
		return *imshow_;
	}
    
    ImShow& imshow(const std::vector<std::vector<std::tuple<float,float,float,float>>>& data) {
		imshow_=std::make_shared<ImShowType<std::tuple<float,float,float,float>>>(data);
        imshow_autoticks();
		return *imshow_;
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
			Color* color = nullptr;
			std::string_view dashes = fmt;
			if (fmt.size()>0) {
				dashes = fmt.substr(1);
				switch (fmt[0]) {
					case 'r': color = &red;     break;
					case 'g': color = &green;   break;
					case 'b': color = &blue;    break;
					case 'k': color = &black;   break;
					case 'w': color = &white;   break;
					case 'c': color = &cyan;    break;
					case 'm': color = &magenta; break;
					case 'y': color = &yellow;  break;
					default: dashes = fmt;
				}
			}
			if (!color) {
				color = cycle[cycle_pos].get();
				cycle_pos = (cycle_pos + 1) % cycle.size();
			}
			return plots.add(Plot(x,y)).color(*color).format(dashes);
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
	
	void savefig(const std::string& name) const {
		std::ofstream f(name);
		f<<svg();
	}	
};

}