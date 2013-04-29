#include "RapidXML.h"
#include "Rdr.h"

#include "Parser.h"

Parser::Parser() {
}
Parser::~Parser() {
}

bool Parser::init() {
	Rdr rdr = Rdr("test.xml");
	bool sucessfulRd = rdr.init();
	
	//if(sucessfulRd) {
	//	std::string rdText = rdr.getRdTxt();
	//}
	//rapidxml::xml_document<> test;

	return sucessfulRd;
}