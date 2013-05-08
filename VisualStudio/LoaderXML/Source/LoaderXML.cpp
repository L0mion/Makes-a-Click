#include <rapidxml.hpp>

//Parser_XML
#include <ParserXML.h>
#include <DocXML.h>

//Util
#include <UtilString.h>

#include "WinDetective.h"

#include "LoaderXML.h"

namespace Loader_XML {
	LoaderXML::LoaderXML() {
		m_winDetective = nullptr;
	}
	LoaderXML::~LoaderXML() {
		if( m_winDetective != nullptr ) {
			delete m_winDetective;
		}
	}

	bool LoaderXML::init() {
		m_winDetective = new WinDetective(
			g_filePathToSearch, 
			g_fileEnding, 
			g_recursiveSearch);
		bool sucessfulLoad = false;
		sucessfulLoad = m_winDetective->init();

		if(sucessfulLoad) {
			parse();
		}

		return sucessfulLoad;
	}

	bool LoaderXML::parse() {
		std::vector<WinDetective::Culprit> foundFiles;
		foundFiles = m_winDetective->getLocatedCulprits();

		bool sucessfulParse = true;
		for(unsigned int i = 0; 
			i<foundFiles.size() && sucessfulParse==true; 
			i++) {
				WinDetective::Culprit c = foundFiles.at(i);
				sucessfulParse = parseXML(
					c.filePath, 
					c.fileName, 
					c.fileEnding);
		}

		return sucessfulParse;
	}

	bool LoaderXML::parseXML(
		const std::wstring filePath, 
		const std::wstring fileName, 
		const std::wstring fileEnding) {
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
				loadXML(docXML);
				if( docXML != nullptr ) {
					delete docXML;
				}
			}

			return sucessfulParse;
	}

	void LoaderXML::loadXML(Parser_XML::DocXML* docXML) {
		/*
		std::string firstNode = docXML->getXML()->first_node()->name();
		
		std::string xmlshit = "";
		for(auto node = docXML->getXML()->first_node();
			node;
			node = node->next_sibling()) {
				xmlshit += node->name();
		}
		
		//std::string firstVal = docXML->getXML()->first_node()->value();

		//std::string xmlshit = "";
		//
		//rapidxml::xml_node<>* node = docXML->getXML()->first_node();
		//for (
		//	rapidxml::xml_attribute<>* attr = node->first_attribute();
		//	attr; 
		//	attr = attr->next_attribute() ) {
		//	xmlshit += attr->name();
		//	xmlshit += attr->value();
		//}

		std::string derp;
		*/
	}
}