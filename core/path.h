#pragma once 

#include "object.h"
#include "presentation-attributes.h"
#include <type_traits>

namespace svg_cpp_plot {

class Path : public Terminal, public PresentationAttributes<Path> {
public:
	//Starting point
	Path(float x, float y) : Terminal("path") 
	{
		std::stringstream sstr;
		sstr<<"M "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		set("fill","none"); set("stroke","black");
	}

	Path(const Path&) = default;
	Path(Path&&) = default;

	//stating_point
	template<typename P>
	Path(const P& p) : Path(std::get<0>(p),std::get<1>(p)) {
		static_assert(is_2d_point_v<P>, "Parameter should be a two dimensional point");
	}

	Path& line_to(float x, float y) {
		std::stringstream sstr;
		sstr<<get("d").value_or("")<<"L "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& horizontal_line_to(float x) {
		std::stringstream sstr;
		sstr<<get("d").value_or("")<<"H "<<x<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& vertial_line_to(float y) {
		std::stringstream sstr;
		sstr<<get("d").value_or("")<<"V "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& close() {
		std::stringstream sstr;
		sstr<<get("d").value_or("")<<"Z ";
		set("d",sstr.str());
		return (*this);
	}

	Path& curve_to(float x1, float y1, float x2, float y2, float x, float y) {
		std::stringstream sstr;
		sstr<<get("d").value_or("")<<"C "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	template<typename P>
	Path& curve_to(const P& p1, const P& p2, const P& p) {
		static_assert(is_2d_point_v<P>, "Parameter should be a two dimensional point");
		return curve_to(std::get<0>(p1), std::get<1>(p1), 
				std::get<0>(p2), std::get<1>(p2), 
				std::get<0>(p), std::get<1>(p));
	}

	Path& curve_string_to(float x2, float y2, float x, float y) {
		std::stringstream sstr;
		sstr<<get("d").value_or("")<<"S "<<x2<<" "<<y2<<" "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& quadratic_curve_to(float x1, float y1, float x, float y) {
		std::stringstream sstr;
		sstr<<get("d").value_or("")<<"Q "<<x1<<" "<<y1<<" "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& quadratic_curve_string_to(float x, float y) {
		std::stringstream sstr;
		sstr<<get("d").value_or("")<<"T "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	BoundingBox bounding_box() const noexcept override { 
		BoundingBox bb;
		char c; float x, y;
		std::stringstream sstr(get("d").value_or(""));

		while (sstr) {
			sstr>>c;
			if (c=='H') { 
				sstr>>x; bb.join({x,y});
			} else if (c=='V') {
				sstr>>y; bb.join({x,y});
			} else if ((c=='M') || (c=='L') || (c=='T')) {
				sstr>>x>>y; bb.join({x,y});
			} else if ((c=='S') || (c=='Q')) {
				sstr>>x>>y; bb.join({x,y});
				sstr>>x>>y; bb.join({x,y});
			} else if (c=='C') {
				sstr>>x>>y; bb.join({x,y});
				sstr>>x>>y; bb.join({x,y});
				sstr>>x>>y; bb.join({x,y});
			}
		}
		bb.expand(get_float("stroke-width",0.0f));
		return bb;
	}
};

}
