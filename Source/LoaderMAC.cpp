#include <LoaderXML.h>

#include "LoaderMAC.h"

LoaderMAC::LoaderMAC() {
	m_loaderXML = nullptr;
}
LoaderMAC::~LoaderMAC() {
	if( m_loaderXML != nullptr ) {
		delete m_loaderXML;
	}
}

bool LoaderMAC::init() {
	m_loaderXML = new Loader_XML::LoaderXML();
	bool sucessfulLoad = m_loaderXML->init();

	return sucessfulLoad;
}