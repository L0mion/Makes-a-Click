#include <stdlib.h> //atoi

#include <LoaderXML.h>
#include <Util.h>

#include "LoaderHeightmap.h"

#include "LoaderMAC.h"

LoaderMAC::LoaderMAC() :
	m_filePathToSearch	( L"../../Resources/Levels/"	),
	m_fileEnding		( L".xml"						),
	m_recursiveSearch	( true							) {
	m_loaderXML = nullptr;
	m_loaderHM	= nullptr;
}
LoaderMAC::~LoaderMAC() {
	if( m_loaderXML!=nullptr ) {
		delete m_loaderXML;
	}
	if( m_loaderHM!=nullptr ) {
		delete m_loaderHM;
	}
}

bool LoaderMAC::init() {
	Util::Mac macDesc;

	//Load xml
	m_loaderXML = new Loader_XML::LoaderXML(
		m_filePathToSearch,
		m_fileEnding,
		m_recursiveSearch);
	bool sucessfulLoad = m_loaderXML->init( macDesc );
	if( sucessfulLoad==true ) {
		//Load heightmap
		std::string path; 
		Util::UtilString::W2Std(m_filePathToSearch, path);
		path += macDesc.heightmap.name + '.' + macDesc.heightmap.ending;
		std::string cntRow	= macDesc.heightmap.cntRow;
		std::string cntCol	= macDesc.heightmap.cntCol;
		std::string scale	= macDesc.heightmap.scale;
		std::string offset	= macDesc.heightmap.offset;

		std::vector<float> heightmap(0);
		m_loaderHM = new LoaderHeightmap(
			path, 
			std::atoi( cntRow.c_str()		), 
			std::atoi( cntCol.c_str()		),
			(float)std::atof( scale.c_str()	),
			(float)std::atof( offset.c_str()));
		sucessfulLoad = m_loaderHM->init(heightmap);
	}

	return sucessfulLoad;
}