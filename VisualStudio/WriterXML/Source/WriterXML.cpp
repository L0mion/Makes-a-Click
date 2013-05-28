#include "DescMAC.h"
#include "Xmler.h"

#include "WriterXML.h"

namespace Writer_XML {
	WriterXML::WriterXML( Util::Mac& p_mac ) {
		m_mac = &p_mac;

		m_xmler = nullptr;
	}
	WriterXML::~WriterXML() {
		if( m_xmler!=nullptr ) {
			delete m_xmler;
		}
	}

	void WriterXML::init( std::string& io_xml ) {
		
		//Observe the temporary path
		std::string TEMPORARY_PATH = "";
		m_xmler = new Xmler(TEMPORARY_PATH, *m_mac);
		m_xmler->init( io_xml );
	}
}