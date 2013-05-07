#include <ParserXML.h>

#include "WinDetective.h"

#include "LoaderXML.h"

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

	for(unsigned int i = 0; i < foundFiles.size(); i++) {
		WinDetective::Culprit c = foundFiles.at(i);
		parseXML(
			c.filePath, 
			c.fileName, 
			c.fileEnding);
	}
}

bool LoaderXML::parseXML(
	const std::wstring filePath, 
	const std::wstring fileName, 
	const std::wstring fileEnding) {
}