#pragma once

#include "attributes.h"
namespace svg_cpp_plot {

ENUM_TYPE(PreserveAspectRatioAlignment) preserve_aspect_ratio_none("none"), preserve_aspect_ratio_xmin_ymin("xMinYMin"),  preserve_aspect_ratio_xmid_ymin("xMidYMin"), preserve_aspect_ratio_xmax_ymin("xMaxYMin"), preserve_aspect_ratio_xmin_ymid("xMinYMid"),  preserve_aspect_ratio_xmid_ymid("xMidYMid"), preserve_aspect_ratio_xmax_ymid("xMaxYMid"), preserve_aspect_ratio_xmin_ymax("xMinYMax"),  preserve_aspect_ratio_xmid_ymax("xMidYMax"), preserve_aspect_ratio_xmax_ymax("xMaxYMax");

ENUM_TYPE(PreserveAspectRatioMeet) preserve_aspect_ratio_meet("meet"), preserve_aspect_ratio_slice("slice");

template<typename T>
class AspectRatioAttributes {
	constexpr T* t() noexcept { return static_cast<T*>(this); } 
	constexpr const T* t() const noexcept { return static_cast<const T*>(this); } 
public:
    T& preserveAspectRatio(const PreserveAspectRatioAlignment& ar, const PreserveAspectRatioMeet& ms) {
        return t()->set("preserveAspectRatio",ar.to_string()+" "+ms.to_string());
    }
    
    T& preserveAspectRatio(const PreserveAspectRatioAlignment& ar) {
        return t()->set("preserveAspectRatio",ar);
    }
};

}
