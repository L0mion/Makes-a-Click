#include "DescMAC.h"
#include "Xmler.h"

#include "WriterXML.h"

namespace Writer_XML {
	WriterXML::WriterXML( DescMAC& p_descMac ) {
		m_descMac = &p_descMac;

		m_xmler = nullptr;
	}
	WriterXML::~WriterXML() {
		//Do nothing.
	}

	void WriterXML::init() {
		std::string TEMPORARY_PATH = ""; //TMEP

		m_xmler = new Xmler(TEMPORARY_PATH, *m_descMac);
		m_xmler->init();
	}
}