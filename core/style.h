#pragma once

#include "object.h"
#include "presentation-attributes.h"

namespace svg_cpp_plot {

class StyleEntry : public AttributesBase, public Attributes<StyleEntry>, public PresentationAttributes<StyleEntry> {
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

class Style : public Object {
	StyleEntry dummy;
	std::list<StyleEntry> entries;
public:
	Style() : Object("style") { }
	
	StyleEntry& add(const StyleEntry& entry) noexcept {
		if (entry.id().empty()) return dummy;
		else { 
			entries.push_back(entry);
			return entries.back();
		}
	}

	StyleEntry& add(const std::string& id) noexcept {
		if (id.empty()) return dummy;
		else {
			entries.push_back(StyleEntry(id));
			return entries.back();
		}
	}

	std::string to_string() const noexcept override {
		std::stringstream sstr;
		sstr<<"<"<<tag()<<" type=\"text/css\">"<<std::endl<<"/* <![CDATA[ */"<<std::endl;
		for (StyleEntry e : entries) sstr<<e.to_string()<<std::endl;
		sstr<<"/* ]]> */"<<std::endl<<"</"<<tag()<<">"<<std::endl;
	       	return sstr.str();	
	}
};

}
