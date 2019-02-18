#pragma once

#include "element.h"
#include "attributes.h"
#include "presentation-attributes.h"
#include "text-presentation-attributes.h"
#include <memory>

namespace svg_cpp_plot {

/**
class StyleEntry : public Object, public ObjectMap, public Attributes<StyleEntry>, public PresentationAttributes<StyleEntry>, public TextPresentationAttributes<StyleEntry> {
public:
	std::string to_string() const noexcept override {
		return map_to_string(": ","; ");
	}
};
**/

class StyleEntry : public AttributesBase, public Attributes<StyleEntry>, public PresentationAttributes<StyleEntry>, public TextPresentationAttributes<StyleEntry> {
	std::string id_;
	//We need them to be shared pointers because otherwise the type is incomplete.
	std::unordered_map<std::string,std::shared_ptr<StyleEntry>> nested;

	StyleEntry& add(const std::string& id) noexcept {
		if (!nested[id]) nested[id]=std::make_shared<StyleEntry>(id);
		else nested[id]->id(id);
		return *(nested[id]);
	}

public:
	constexpr const std::string& id() const noexcept { return id_; }
	StyleEntry& id(const std::string& i) noexcept { id_=i; return *this; }

	StyleEntry& nest(const std::string& id) noexcept {
		return add(std::string(" ")+id);
	}

	StyleEntry& pseudoclass(const std::string& id) noexcept {
		return add(std::string(":")+id);
	}

	StyleEntry& hover() noexcept {
		return pseudoclass("hover");
	}


	StyleEntry(const std::string& id = "") : id_(id) {}
	//Nested styles are ignored if there is no ID. With no ID, a styleEntry can be attached to any element.
	std::string to_string(const std::string& prefix = "") const noexcept {
		std::stringstream sstr;
		if (!id().empty()) sstr<<prefix+id()<<" {"<<std::endl;
		sstr<<map_to_string(": ","; ");
		if (!id().empty()) sstr<<std::endl<<"}"<<std::endl;
		if (!id().empty()) for( auto const& [key, style] : nested ) sstr<<style->to_string(prefix+id())<<std::endl;
		return sstr.str();
	}		
};


class Style : public NotTerminal, public Attributes<Style> {
	std::unordered_map<std::string,StyleEntry> entries;
public:
	Style() : NotTerminal("style") { set("type","text/css"); }

	StyleEntry& add(const StyleEntry& entry) noexcept {
		return entries[entry.id()].merge_with(entry);
	}

	StyleEntry& add(const std::string& id) noexcept {
		return entries[id].id(id);
	}
	
	StyleEntry& add_class(const std::string& classname) noexcept {
		if (classname.empty()) return add("");
		else return add(std::string(".")+classname);
	}

	std::string content() const noexcept override {
		std::stringstream sstr;
		sstr<<"/* <![CDATA[ */"<<std::endl;
		for( auto const& [key, style] : entries ) sstr<<style.to_string()<<std::endl;
		sstr<<"/* ]]> */"<<std::endl<<std::endl;
	       	return sstr.str();	
	}
};

}
