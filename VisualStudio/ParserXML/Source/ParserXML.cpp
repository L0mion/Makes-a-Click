#include "Rdr.h"
#include "RdrXML.h"

#include "ParserXML.h"

ParserXML::ParserXML(std::string filePath, std::string fileName) {
	m_filePath		= filePath;
	m_fileName		= fileName;
	m_filePathFull	= filePath + fileName;
}
ParserXML::~ParserXML() {
	//Do nothing.
}

bool ParserXML::init() {
	Rdr rdr = Rdr(m_filePathFull);
	Rdr::RdStatuses rdStatus = Rdr::RdStatuses_OK;
	rdStatus = rdr.init();

	bool sucessfulParse = true;
	if( rdStatus == Rdr::RdStatuses_OK ) {
		std::string rdText = rdr.getRdTxt();
		parse(rdText);
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

void ParserXML::parse(std::string txtToParse) {
	RdrXML parserXML(txtToParse);

	bool sucessfulParse = false;
	sucessfulParse = parserXML.init();

	if( sucessfulParse ) {

	}
}