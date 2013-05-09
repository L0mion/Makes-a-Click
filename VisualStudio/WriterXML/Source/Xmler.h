#pragma once

#include <string>
#include <rapidxml.hpp>

#include "SettingsWriterXML.h"

typedef rapidxml::xml_node<WriterXMLCharType> XmlNode;

namespace Writer_XML {
	class DescMAC;
}

namespace Writer_XML {
	class Xmler {
	public:
		Xmler( DescMAC& descMac );
		~Xmler();

		void init();
	protected:
	private:
		XmlNode* allocNode( const std::string& p_s );
		char* allocString( const std::string& p_s);

		DescMAC* m_descMAC;
		rapidxml::xml_document<> m_xml;
	};
}