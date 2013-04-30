#include <exception>

#include "ParserXML.h"

ParserXML::ParserXML(std::string xmlRaw) {
	m_xmlRaw = xmlRaw;

	m_xmlDoc = nullptr;
}
ParserXML::~ParserXML() {
	if( m_xmlDoc )
		delete m_xmlDoc;
}

bool ParserXML::init() {
	bool sucessfulParse = true;

	//Create our representation of the XML-tree.
	m_xmlDoc = new rapidxml::xml_document<ParserXMLCharType>(); //Character Type defaults to Char because of esoteric template-stuff.
	
	//Parse the XML-document into the tree. May throw a parse_error exception.
	try {
		m_xmlDoc->parse<0>((char*)m_xmlRaw.c_str()); //Potential flags are in place of '0'.
	} catch (rapidxml::parse_error e) {
		//Find out what has happened, and where within the given text the error has occurred.
		std::string parse_error_what(e.what());
		//ParserXMLCharType* parse_error_where = (ParserXMLCharType*)e.where();	

		sucessfulParse = false;

		//Register error somehow.
	}

	return sucessfulParse;
}