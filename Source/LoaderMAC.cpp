#include <LoaderXML.h>
#include <Util.h>

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
	Util::Mac temp; //This ought to be a member var or inout

	m_loaderXML = new Loader_XML::LoaderXML();
	bool sucessfulLoad = m_loaderXML->init( temp );

	return sucessfulLoad;
}