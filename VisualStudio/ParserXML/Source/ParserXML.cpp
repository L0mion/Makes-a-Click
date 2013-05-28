#include "Rdr.h"
#include "RdrXML.h"
#include "DocXML.h"

#include "ParserXML.h"

namespace Parser_XML {
	ParserXML::ParserXML(char* p_xml) {
			m_xml = p_xml;

			m_docXML = nullptr;
			m_memManagement = true;
	}
	ParserXML::~ParserXML() {
		if( m_memManagement == true ) {
			if( m_docXML != nullptr ) {
				delete m_docXML;
			}
		}
	}

	bool ParserXML::init() {
		m_docXML = new DocXML(m_xml);

		RdrXML rdrXML(m_docXML);

		bool sucessfulParse = false;
		sucessfulParse = rdrXML.init();

		return sucessfulParse;
	}

	void ParserXML::getDocXML(Parser_XML::DocXML** inout_docXML) {
		*inout_docXML = m_docXML;
		m_memManagement = false;
	}
}