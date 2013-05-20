#include "DocXML.h"

namespace Parser_XML {
	DocXML::DocXML(const std::string rawXML) {
		m_rawXML = rawXML;

		m_xml = nullptr;
	}
	DocXML::~DocXML() {
		if( m_xml != nullptr ) {
			delete m_xml;
		}
	}

	bool DocXML::isValid() {
		bool valid = true;
		if( m_xml == nullptr ) {
			valid = false;
		}

		return valid;
	}

	//Gets
	std::string& DocXML::getRawXML() { 
		return m_rawXML; 
	}
	rapidxml::xml_document<>* DocXML::getXML() const {
		return m_xml;
	}

	//Sets
	void DocXML::setXML(rapidxml::xml_document<>* p_xml) {
		m_xml = p_xml;
	}
}