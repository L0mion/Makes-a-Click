#pragma once

typedef char ParserXMLCharType;

#include <string>

#include "RapidXML.h"

class ParserXML {
public:
	ParserXML(std::string xmlRaw);
	~ParserXML();

	bool init();
protected:
private:
	std::string m_xmlRaw;
	rapidxml::xml_document<>* m_xmlDoc;
};