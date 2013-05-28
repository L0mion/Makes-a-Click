#include <stdlib.h> //atoi
#include <cstdlib> //atof

#include <LoaderXML.h>
#include <Util.h>
#include <Tartar.h>

#include "LoaderHeightmap.h"
#include "LoaderRaw.h"

#include "LoaderMAC.h"

//TEMP
#include "WriterZlib.h"

LoaderMAC::LoaderMAC() :
	m_filePathToSearch	( L"../../Resources/Levels/"	),
	m_fileEnding		( L".xml"						),
	m_recursiveSearch	( true							) {
	m_loaderXML = nullptr;
	m_loaderHM	= nullptr;
	m_loaderBM = nullptr;
}
LoaderMAC::~LoaderMAC() {
	if( m_loaderXML!=nullptr ) {
		delete m_loaderXML;
	}
	if( m_loaderHM!=nullptr ) {
		delete m_loaderHM;
	}
	if( m_loaderBM!=nullptr ) {
		delete m_loaderBM;
	}

}

bool LoaderMAC::init( Mac& io_result ) {

	std::string path	= "../../Resources/Levels/";
	std::string tarpath = path + "TestLvl"; tarpath += ".tar";

	Tartar::Tar tar;
	Tartar::TarUncook* taruncook = new Tartar::TarUncook( tarpath.c_str() );
	bool oktar  = taruncook->init(tar);

	Util::MacDesc macDesc;
	m_loaderXML = new Loader_XML::LoaderXML( tar.getFiles().at(0)->fileData );
	oktar = m_loaderXML->init( macDesc );

	std::string herpa;

	//Load xml
	//m_loaderXML = new Loader_XML::LoaderXML(
	//	m_filePathToSearch,
	//	m_fileEnding,
	//	m_recursiveSearch);
	//bool sucessfulLoad = m_loaderXML->init( io_result.macDesc );
	//if( sucessfulLoad==true ) {
	//	//Load heightmap
	//	std::string path; 
	//	Util::UtilString::W2Std(m_filePathToSearch, path);
	//	path += io_result.macDesc.heightmap.name + '.' + io_result.macDesc.heightmap.ending;
	//	std::string cntRow	= io_result.macDesc.heightmap.cntRow;
	//	std::string cntCol	= io_result.macDesc.heightmap.cntCol;
	//	std::string scale	= io_result.macDesc.heightmap.scale;
	//	std::string offset	= io_result.macDesc.heightmap.offset;
	//
	//	m_loaderHM = new LoaderHeightmap(
	//		path, 
	//		std::atoi( cntRow.c_str()		), 
	//		std::atoi( cntCol.c_str()		),
	//		(float)std::atof( scale.c_str()	),
	//		(float)std::atof( offset.c_str()));
	//	sucessfulLoad = m_loaderHM->init( io_result.heightmap );
	//
	//	//Load blendmap
	//	if( sucessfulLoad==true ) {
	//		std::string bmName = io_result.macDesc.blendmap.name;
	//		unsigned int bmRow = std::atoi(io_result.macDesc.blendmap.width.c_str());
	//		unsigned int bmCol = std::atoi(io_result.macDesc.blendmap.height.c_str());
	//	
	//		std::vector<unsigned char> raw;
	//	
	//		Util::UtilString::W2Std(m_filePathToSearch, path);
	//		path += bmName + '.' + "map";
	//		m_loaderBM = new LoaderRaw( 
	//			path, 
	//			bmRow * 2, 
	//			bmCol * 2);
	//		sucessfulLoad = m_loaderBM->init( raw );
	//	
	//		uglyRawToBlendmap( raw, bmRow, bmCol, io_result.blendmap );
	//	}
	//}

	return oktar;
}

void LoaderMAC::uglyRawToBlendmap( std::vector<unsigned char>& p_raw, unsigned int p_row, unsigned int p_col, std::vector<Util::Texel>& io_bm ) {
	unsigned int numTexels= p_row * p_col;

	io_bm.resize(numTexels);

	int t = 0;
	for( unsigned int i = 0; i < numTexels * 4; i+=4 ) {
		io_bm[t] = Util::Texel(
			(float)p_raw[i]		/ 255.0f,
			(float)p_raw[i+1]	/ 255.0f,
			(float)p_raw[i+2]	/ 255.0f,
			(float)p_raw[i+3]	/ 255.0f);
		t++;
	}
}