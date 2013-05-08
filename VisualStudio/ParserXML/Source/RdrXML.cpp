#include <exception>

#include "DocXML.h"

#include "RdrXML.h"

namespace Parser_XML {
	RdrXML::RdrXML(DocXML* inout_docXML) {
		m_inout_docXML = inout_docXML;
	}
	RdrXML::~RdrXML() {
		//Do nothing.
	}

	bool RdrXML::init() {
		bool sucessfulParse = true;

		//Create our representation of the XML-tree.
		auto xmlDoc = new rapidxml::xml_document<ParserXMLCharType>(); //Character Type defaults to Char because of esoteric template-stuff.

		//Parse the XML-document into the tree. May throw a parse_error exception.
		try {
			xmlDoc->parse<0>((char*)m_inout_docXML->getRawXML().c_str()); //Potential flags are in place of '0'.
		} catch (rapidxml::parse_error e) {
			//Find out what has happened, and where within the given text the error has occurred.
			std::string parse_error_what(e.what());
			//ParserXMLCharType* parse_error_where = (ParserXMLCharType*)e.where();	

			sucessfulParse = false;

			//Register error somehow.
		}

		if( sucessfulParse == true ) {
			m_inout_docXML->setXML(xmlDoc);
		} else /*sucessfulParse == false*/ {
			if( xmlDoc != nullptr ) {
				delete xmlDoc;
			}
		}

		return sucessfulParse;
	}
}