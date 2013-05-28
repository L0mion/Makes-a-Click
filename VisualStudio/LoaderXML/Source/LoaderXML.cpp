#include <rapidxml.hpp>

//Parser_XML
#include <ParserXML.h>
#include <DocXML.h>

//Util
#include <Util.h>

#include "WinDetective.h"
#include "InterpreterXML.h"

#include "LoaderXML.h"

namespace Loader_XML {
	LoaderXML::LoaderXML( char* p_xml ) {
		m_xml = p_xml;
	}
	LoaderXML::~LoaderXML() {
	}

	bool LoaderXML::init( Util::MacDesc& inout_result ) {
		bool sucessfulParse = true;

		Parser_XML::DocXML* docXML = nullptr;
		Parser_XML::ParserXML parserXML(m_xml);
		sucessfulParse = parserXML.init();
		parserXML.getDocXML(&docXML);

		//Load data from parsed XML-document into resulting type
		InterpreterXML interpreter( docXML->getXML() );
		sucessfulParse = interpreter.init( inout_result );

		//Clear some memory
		if( docXML != nullptr ) {
			delete docXML;
		}

		return sucessfulParse;
	}
}