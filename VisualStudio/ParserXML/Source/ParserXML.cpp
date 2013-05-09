#include "Rdr.h"
#include "RdrXML.h"
#include "DocXML.h"

#include "ParserXML.h"

namespace Parser_XML {
	ParserXML::ParserXML(
		std::string p_filePath, 
		std::string p_fileName) {
			m_filePath		= p_filePath;
			m_fileName		= p_fileName;
			m_filePathFull	= m_filePath + m_fileName;

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
		Rdr rdr = Rdr(m_filePathFull);
		Rdr::RdStatuses rdStatus = Rdr::RdStatuses_OK;
		rdStatus = rdr.init();

		bool sucessfulParse = true;
		if( rdStatus == Parser_XML::Rdr::RdStatuses_OK ) {
			std::string rdText = rdr.getRdTxt();
			sucessfulParse = parse(rdText);
		} else {
			//Do some appropriate error handling/printing.
			switch( rdStatus ) {
			case Rdr::RdStatuses_NOT_FOUND:
				break;
			case Rdr::RdStatuses_EOF:
				break;
			case Rdr::RdStatuses_BAD:
				break;
			case Rdr::RdStatuses_FAIL:
				break;
			}
			sucessfulParse = false;
		}

		return sucessfulParse;
	}

	void ParserXML::getDocXML(Parser_XML::DocXML** inout_docXML) {
		*inout_docXML = m_docXML;
		m_memManagement = false;
	}

	bool ParserXML::parse(std::string p_txtToParse) {
		m_docXML = new DocXML(p_txtToParse);

		RdrXML rdrXML(m_docXML);

		bool sucessfulParse = false;
		sucessfulParse = rdrXML.init();

		return sucessfulParse;
	}
}