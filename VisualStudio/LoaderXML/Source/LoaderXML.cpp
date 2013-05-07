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

void LoaderXML::init() {
	m_winDetective = new WinDetective(
		g_filePathToSearch, 
		g_fileEnding, 
		g_recursiveSearch);
	bool sucessfulLoad = false;
	sucessfulLoad = m_winDetective->init();
}