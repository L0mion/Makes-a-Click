#pragma once

#include <string>

#include <rapidxml.hpp>

namespace Parser_XML {
	class DocXML {
	public:
		DocXML(const std::string rawXML);
		~DocXML();

		bool isValid();

		//Gets
		std::string& getRawXML();
		rapidxml::xml_document<>* getXML() const;

		//Sets
		void setXML(rapidxml::xml_document<>* p_xml);
	protected:
	private:
		rapidxml::xml_document<>* m_xml;
		std::string m_rawXML;
	};
}