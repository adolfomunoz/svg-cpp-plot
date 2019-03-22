#pragma once

#include "../core/element.h"
#include "attributes.h"
#include "presentation-attributes.h"
#include "text-presentation-attributes.h"
#include <memory>

namespace svg_cpp_plot {

class StyleEntryBase : public Object, public AttributesBase {
public:
	std::string to_string() const noexcept override {
		return map_to_string(": ","; ");
	}
};

class ElementStyle : public StyleEntryBase, public Attributes<ElementStyle>, public PresentationAttributes<ElementStyle>, public TextPresentationAttributes<ElementStyle> {
};

class Style;
class StyleEntry;
StyleEntry& add(Style& style, const std::string& id);

class StyleEntry : public StyleEntryBase, public Attributes<StyleEntry>, public PresentationAttributes<StyleEntry>, public TextPresentationAttributes<StyleEntry> {
	std::string id_;
	Style& style_;
public:
	StyleEntry(const std::string& id, Style& style) : id_(id), style_(style) {}
	constexpr const std::string& id() const noexcept { return id_; }

	StyleEntry& nest(const std::string& nested) noexcept {
		return add(style_,id()+" "+nested);
	}

	StyleEntry& pseudoclass(const std::string& pc) noexcept {
		return add(style_,id()+":"+pc);
	}

	StyleEntry& hover() noexcept {
		return pseudoclass("hover");
	}
};

class Style : public NotTerminal, public Attributes<Style> {
public:
	ObjectMap<StyleEntry> entries;
public:
	Style() : NotTerminal("style") { set("type","text/css"); }

	StyleEntry& add(const std::string& id) noexcept {
		return entries.get_or_set(id,StyleEntry(id,*this));
	}
	
	StyleEntry& add_class(const std::string& classname) noexcept {
		if (classname.empty()) return add("");
		else return add(std::string(".")+classname);
	}

	std::string content() const noexcept override {
		std::stringstream sstr;
		sstr<<"/* <![CDATA[ */"<<std::endl;
		sstr<<entries.map_to_string(" {\n","\n}\n\n");
		sstr<<"/* ]]> */"<<std::endl;
	       	return sstr.str();	
	}
};

inline StyleEntry& add(Style& style, const std::string& id) {
	return style.add(id);
}

}
