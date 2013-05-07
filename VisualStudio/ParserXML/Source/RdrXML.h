#pragma once

typedef char ParserXMLCharType;

#include <string>

#include "RapidXML.h"

class RdrXML {
public:
	RdrXML(std::string xmlRaw);
	~RdrXML();

	bool init();
protected:
private:
	std::string m_xmlRaw;
	rapidxml::xml_document<>* m_xmlDoc;
};