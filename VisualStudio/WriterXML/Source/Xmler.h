#pragma once

#include <string>
#include <rapidxml.hpp>

#include "SettingsWriterXML.h" //WriterXMLCharType

typedef rapidxml::xml_node<WriterXMLCharType> XmlNode;
typedef rapidxml::xml_document<WriterXMLCharType> XmlDoc;

namespace Writer_XML {
	class DescMAC;
	class Wtr;
}

namespace Writer_XML {
	class Xmler {
	public:
		Xmler( std::string& p_pathMac, DescMAC& p_descMac );
		~Xmler();

		bool init();
	protected:
	private:
		bool print();

		void getXmlAsString( std::string& p_s );
		XmlNode* allocNode( const std::string& p_s );
		char* allocString( const std::string& p_s);

		Wtr* m_wtr;

		std::string* m_pathMac;
		DescMAC* m_descMAC;
		rapidxml::xml_document<>* m_xml;
	};
}