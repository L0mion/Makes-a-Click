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

bool LoaderMAC::init( Mac& io_result ) {
	//Load xml
	m_loaderXML = new Loader_XML::LoaderXML(
		m_filePathToSearch,
		m_fileEnding,
		m_recursiveSearch);
	bool sucessfulLoad = m_loaderXML->init( io_result.macDesc );
	if( sucessfulLoad==true ) {
		//Load heightmap
		std::string path; 
		Util::UtilString::W2Std(m_filePathToSearch, path);
		path += io_result.macDesc.heightmap.name + '.' + io_result.macDesc.heightmap.ending;
		std::string cntRow	= io_result.macDesc.heightmap.cntRow;
		std::string cntCol	= io_result.macDesc.heightmap.cntCol;
		std::string scale	= io_result.macDesc.heightmap.scale;
		std::string offset	= io_result.macDesc.heightmap.offset;

		m_loaderHM = new LoaderHeightmap(
			path, 
			std::atoi( cntRow.c_str()		), 
			std::atoi( cntCol.c_str()		),
			(float)std::atof( scale.c_str()	),
			(float)std::atof( offset.c_str()));
		sucessfulLoad = m_loaderHM->init( io_result.heightmap );
	}

	return sucessfulLoad;
}