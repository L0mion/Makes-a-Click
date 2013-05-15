#include <string>
#include <UtilDefinitionMAC.h>
#include <rapidxml_print.hpp>

#include "SettingsWriterXML.h"
#include "DescMAC.h"
#include "Wtr.h"

#include "Xmler.h"

namespace Writer_XML {
	Xmler::Xmler( std::string& p_pathMac, DescMAC& p_descMac ) {
		m_pathMac = &p_pathMac;
		m_descMAC = &p_descMac;

		m_xml = nullptr;
		m_wtr = nullptr;
	}
	Xmler::~Xmler() {
		// Not sure about what we're going to do here.
		// But it ought to be something like this:
		if( m_xml != nullptr ) {
			delete m_xml;
		}
		if( m_wtr != nullptr ) {
			delete m_wtr;
		}
	}

	bool Xmler::init() {
		m_xml = new XmlDoc();

		//Create the original node:	
		//m_xml.append_node(allocNode(Util::DefinitionMac_LEVEL));
		m_xml->append_node(allocNode("1"));
		m_xml->append_node(allocNode("2"));
		m_xml->append_node(allocNode("3"));
		m_xml->append_node(allocNode("4"));

		bool sucessfulPrint = print();

		return sucessfulPrint;

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
	}

	bool Xmler::print() {
		std::string xml_string;
		getXmlAsString(xml_string);

		std::string fileName = m_descMAC->getNameFile();
		m_wtr = new Wtr( 
			*m_pathMac, 
			fileName, 
			xml_string );
		bool sucessfulPrint = m_wtr->init();

		return sucessfulPrint;
	}

	void Xmler::getXmlAsString( std::string& p_s ) {
		rapidxml::print(
			std::back_inserter(p_s), 
			*m_xml, 
			0);
	}

	XmlNode* Xmler::allocNode( const std::string& p_s ) {
		return m_xml->allocate_node(
			rapidxml::node_element,
			allocString(p_s));
	}

	char* Xmler::allocString( const std::string& p_s ) {
		return m_xml->allocate_string(p_s.c_str());
	}
}