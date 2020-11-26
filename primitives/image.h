#pragma once 

#include "../core/element.h"
#include "../attributes/style-attributes.h"
#include "../attributes/graphical-attributes.h"
#include "../attributes/geometry-attributes.h"

namespace svg_cpp_plot {
    
ENUM_TYPE(PreserveAspectRatio) preserve_aspect_ratio_none("none"), preserve_aspect_ratio_xmin_ymin("xMinYMin"),  preserve_aspect_ratio_xmid_ymin("xMidYMin"), preserve_aspect_ratio_xmax_ymin("xMaxYMin"), preserve_aspect_ratio_xmin_ymid("xMinYMid"),  preserve_aspect_ratio_xmid_ymid("xMidYMid"), preserve_aspect_ratio_xmax_ymid("xMaxYMid"), preserve_aspect_ratio_xmin_ymax("xMinYMax"),  preserve_aspect_ratio_xmid_ymax("xMidYMax"), preserve_aspect_ratio_xmax_ymax("xMaxYMax");

class Image : public Terminal, public Attributes<Image>, public GraphicalAttributes<Image>, public StyleAttributes<Image>, public GeometryAttributes<Image> {
public:
	std::string href() const noexcept { return get_default("href",std::string("")); }
	Image& href(const std::string& v) noexcept { 
		return set("href",v).set("data-href",v).set("xlink:href",v); 
	}  //data-href is for reloading (if needed). xlink:href is for inkscape compatibility

	Image(const std::string& name) : Terminal("image")  {
		href(name);
	}

	BoundingBox bounding_box() const noexcept override { 
		return BoundingBox(x(),y(),x()+width(),y()+width());
	}
    
    Image& viewBox(const BoundingBox& bb) {
		set("viewBox",bb);
		return (*this);
	}
    
    BoundingBox viewBox() const {
		return get_default("viewBox",BoundingBox());
	}
    
    Image& preserveAspectRatio(const PreserveAspectRatio& ar) {
        set("preserveAspectRatio",ar);
        return (*this);
    }
};

}
