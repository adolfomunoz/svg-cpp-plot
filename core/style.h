#pragma once

#include "object.h"
#include "attributes.h"
#include "presentation-attributes.h"
#include "text-presentation-attributes.h"
#include <map>

namespace svg_cpp_plot {

class StyleEntry : public AttributesBase, public Attributes<StyleEntry>, public PresentationAttributes<StyleEntry>, public TextPresentationAttributes<StyleEntry> {
	std::string id_;
public:
	constexpr const std::string& id() const noexcept { return id_; }
	StyleEntry& id(const std::string& i) noexcept { id_=i; return *this; }

	StyleEntry(const std::string& id = "") : id_(id) {}
	std::string to_string() noexcept {
		std::stringstream sstr;
		if (!id().empty()) sstr<<id()<<" {"<<std::endl;
		sstr<<attributes_to_string(": ","; ");
		if (!id().empty()) sstr<<std::endl<<"}";
		return sstr.str();
	}		
};


class Style : public NotTerminal {
	std::map<std::string,StyleEntry> entries;
public:
	Style() : NotTerminal("style") { (*this)["type"]="text/css"; }
	
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
		for (auto e : entries) sstr<<e.second.to_string()<<std::endl;
		sstr<<"/* ]]> */"<<std::endl<<std::endl;
	       	return sstr.str();	
	}
};

}
