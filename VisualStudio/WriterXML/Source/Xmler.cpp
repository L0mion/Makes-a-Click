#include <string>
#include <UtilDefinitionMAC.h>
#include <rapidxml_print.hpp>

#include "SettingsWriterXML.h"

#include "Xmler.h"

namespace Writer_XML {
	Xmler::Xmler( DescMAC& descMac ) {
		m_descMAC = &descMac;
	}
	Xmler::~Xmler() {
		//Do nothing.
	}

	void Xmler::init() {
		m_xml = rapidxml::xml_document<WriterXMLCharType>();

		//Create the original node:	
		//m_xml.append_node(allocNode(Util::DefinitionMac_LEVEL));

		m_xml.append_node(allocNode("1"));
		m_xml.append_node(allocNode("2"));
		m_xml.append_node(allocNode("3"));
		m_xml.append_node(allocNode("4"));
		char buffer[4096];                 
		char *end = rapidxml::print(buffer, m_xml, 0); 
		*end = 0;
		std::string xmlpreview(buffer);
		std::string derp = "";

		//std::string nodeName = "StringWithLifeTimeAsDocument";
		//// Allocate string and copy name into it
		//char* nodeNameChar = doc.allocate_string(nodeName.c_str());
		//// Set node name to node_name
		//rapidxml::xml_node<>* node = doc.allocate_node(
		//	rapidxml::node_element, 
		//	nodeNameChar);
		//doc.append_node(node);
		//
		//std::string anotherNodeName = "AnotherStringEtc.";
		//char* anotherNodeNameChar = doc.allocate_string(anotherNodeName.c_str());
		//rapidxml::xml_node<>* anotherNode = doc.allocate_node(
		//	rapidxml::node_element,
		//	anotherNodeNameChar);
		//doc.append_node(anotherNode);
		//
		//char buffer[4096];                 
		//char *end = rapidxml::print(buffer, doc, 0); 
		//*end = 0;
		//std::string xmlpreview(buffer);
	}

	XmlNode* Xmler::allocNode( const std::string& p_s ) {
		return m_xml.allocate_node(
			rapidxml::node_element,
			allocString(p_s));
	}

	char* Xmler::allocString( const std::string& p_s ) {
		return m_xml.allocate_string(p_s.c_str());
	}
}