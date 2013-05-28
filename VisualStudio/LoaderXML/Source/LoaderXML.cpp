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
	LoaderXML::LoaderXML(
		std::wstring	p_filePathToSearch,
		std::wstring	p_fileEnding,
		bool			p_recursiveSearch) {
		m_filePathToSearch	= p_filePathToSearch;
		m_fileEnding		= p_fileEnding;
		m_recursiveSearch	= p_recursiveSearch;

		m_winDetective = nullptr;
	}
	LoaderXML::~LoaderXML() {
		if( m_winDetective != nullptr ) {
			delete m_winDetective;
		}
	}

	bool LoaderXML::init( Util::MacDesc& inout_result ) {
		m_winDetective = new WinDetective(
			m_filePathToSearch, 
			m_fileEnding, 
			m_recursiveSearch);
		bool sucessfulLoad = false;
		sucessfulLoad = m_winDetective->init();

		if(sucessfulLoad) {
			parse( inout_result );
		}

		return sucessfulLoad;
	}

	bool LoaderXML::parse( Util::MacDesc& inout_result ) {
		std::vector<WinDetective::Culprit> foundFiles;
		foundFiles = m_winDetective->getLocatedCulprits();

		bool sucessfulParse = true;

		//This needs to be fixed somehow, append result to vector of MACs instead of a single inout-parameter?
		for(unsigned int i = 0; 
			i<foundFiles.size() && sucessfulParse==true; 
			i++) {
				WinDetective::Culprit c = foundFiles.at(i);
				sucessfulParse = parseXML(
					c.filePath, 
					c.fileName, 
					c.fileEnding,
					inout_result );
		}

		return sucessfulParse;
	}

	bool LoaderXML::parseXML(
		const std::wstring filePath, 
		const std::wstring fileName, 
		const std::wstring fileEnding,
		Util::MacDesc& inout_result ) {
			std::string filePathStd; 
			std::string fileNameStd;
			filePathStd = Util::UtilString::W2Std(filePath, filePathStd);
			fileNameStd = Util::UtilString::W2Std(fileName, fileNameStd);

			//Parse xml.
			bool sucessfulParse = false;
			Parser_XML::ParserXML parserXML(filePathStd, fileNameStd);
			sucessfulParse = parserXML.init();

			if(sucessfulParse == true) {
				Parser_XML::DocXML* docXML = nullptr;
				parserXML.getDocXML(&docXML);
				
				//Load data from parsed XML-document into resulting type
				InterpreterXML interpreter( docXML->getXML() );
				sucessfulParse = interpreter.init( inout_result );

				//Clear some memory
				if( docXML != nullptr ) {
					delete docXML;
				}
			}

			return sucessfulParse;
	}
}