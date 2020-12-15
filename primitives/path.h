#pragma once 

#include "../core/element.h"
#include "../attributes/presentation-attributes.h"
#include "../attributes/style-attributes.h"
#include "../attributes/graphical-attributes.h"
#include <type_traits>

namespace svg_cpp_plot {

class Path : public Terminal, public Attributes<Path>, public GraphicalAttributes<Path>, public StyleAttributes<Path>, public PresentationAttributes<Path> {
public:
	//Starting point
	Path(float x, float y) : Terminal("path") 
	{
		std::stringstream sstr;
		sstr<<"M "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		//fill(none); 
	}

	std::tuple<float, float> first_point() const {
		std::istringstream sstr(get_default("d",std::string("")));
		std::tuple<float,float> p; char M;
		sstr>>M>>std::get<0>(p)>>std::get<1>(p);
		return p;
	}

	Path(const Path&) = default;
	Path(Path&&) = default;
	Path& operator=(const Path&) = default;
	Path& operator=(Path&&) = default;

	//stating_point
	template<typename P>
	Path(const P& p) : Path(std::get<0>(p),std::get<1>(p)) {
		static_assert(is_2d_point_v<P>, "Parameter should be a two dimensional point");
	}

	Path& line_to(float x, float y) {
		std::stringstream sstr;
		sstr<<get_default("d",std::string(""))<<"L "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& horizontal_line_to(float x) {
		std::stringstream sstr;
		sstr<<get_default("d",std::string(""))<<"H "<<x<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& vertical_line_to(float y) {
		std::stringstream sstr;
		sstr<<get_default("d",std::string(""))<<"V "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& close() {
		std::stringstream sstr;
		sstr<<get_default("d",std::string(""))<<"Z ";
		set("d",sstr.str());
		return (*this);
	}

	Path& curve_to(float x1, float y1, float x2, float y2, float x, float y) {
		std::stringstream sstr;
		sstr<<get_default("d",std::string(""))<<"C "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<x<<" "<<y<<" ";
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
		sstr<<get_default("d",std::string(""))<<"S "<<x2<<" "<<y2<<" "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& quadratic_curve_to(float x1, float y1, float x, float y) {
		std::stringstream sstr;
		sstr<<get_default("d",std::string(""))<<"Q "<<x1<<" "<<y1<<" "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	Path& quadratic_curve_string_to(float x, float y) {
		std::stringstream sstr;
		sstr<<get_default("d",std::string(""))<<"T "<<x<<" "<<y<<" ";
		set("d",sstr.str());
		return (*this);
	}

	BoundingBox bounding_box() const noexcept override { 
		BoundingBox bb;
		char c; float x, y;
		std::stringstream sstr(get_default("d",std::string("")));

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
