#pragma once

#include <string>
#include <rapidxml.hpp>

#include "SettingsWriterXML.h" //WriterXMLCharType

typedef rapidxml::xml_node<WriterXMLCharType> XmlNode;
typedef rapidxml::xml_document<WriterXMLCharType> XmlDoc;
typedef rapidxml::xml_attribute<WriterXMLCharType> XmlAtt;

namespace Util {
	struct Mac;
}

namespace Writer_XML {
	class Wtr;
}

namespace Writer_XML {
	class Xmler {
	public:
		Xmler( std::string& p_pathMac, Util::Mac& p_mac );
		~Xmler();

		void init( std::string& io_xml );
	protected:
	private:
		void appendAtt( XmlNode* p_n, const std::string& p_a, const std::string& p_val );

		void getXmlAsString( std::string& p_s );
		XmlNode* allocNode( const std::string& p_s );
		XmlAtt* allocAtt( const std::string& p_a, const std::string& p_b );
		char* allocString( const std::string& p_s);

		Wtr* m_wtr;

		std::string* m_pathMac;
		Util::Mac* m_mac;
		rapidxml::xml_document<>* m_xml;
	};
}