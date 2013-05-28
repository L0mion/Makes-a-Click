#include <WriterXML.h>
#include <Wtr.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <Tartar.h> //awwwyeah
#include <string>

#include "Mac.h"

#include "WriterMAC.h"

WriterMAC::WriterMAC( Mac& io_mac ) {
	m_mac = &io_mac;
}
WriterMAC::~WriterMAC() {
	// Do nothing.
}

bool WriterMAC::init() {
	Util::MacDesc macmac = m_mac->macDesc;
	
	// Get xml
	std::string xml;
	Writer_XML::WriterXML* writerXML = new Writer_XML::WriterXML( macmac );
	writerXML->init( xml );
	
	// Get raw heightmap
	std::vector<unsigned char> rawHM;
	uglyHeightmapToRaw( 
		m_mac->heightmap, 
		(float)std::atof(macmac.heightmap.scale.c_str()),  //HAHA
		(float)std::atof(macmac.heightmap.offset.c_str()), //HAHA 
		rawHM );

	// Get raw blendmap
	std::vector<unsigned char> rawBM;
	uglyBlendmapToRaw(
		m_mac->blendmap, 
		std::atof(macmac.blendmap.width.c_str()), 
		std::atof(macmac.blendmap.height.c_str()), 
		rawBM);

	std::string xmlName = m_mac->macDesc.name + ".xml";
	std::string hmName = macmac.heightmap.name;
	std::string bmName = macmac.blendmap.name;

	std::string path	= "../../Resources/Levels/";

	std::string tarpath = path + m_mac->macDesc.name + ".tar";

	// Archive into tar
	//Tartar::File f;
	//Tartar::TarCookMem* tarcook = new Tartar::TarCookMem( f );
	//tarcook->cook( xmlName.c_str(), xml.c_str(), xml.size() );
	//tarcook->cook( rawName.c_str(), &raw[0], raw.size() );
	//
	//Writer_XML::Wtr* wtr = new Writer_XML::Wtr( tarpath.c_str(), f.fileData, f.fileSize );
	//bool ok = wtr->init( true );

	std::string pathxml = path + xmlName;
	Writer_XML::Wtr* wtr = new Writer_XML::Wtr( pathxml.c_str(), (unsigned char*)xml.c_str(), xml.size() );
	bool ok = wtr->init(true);
	delete wtr;

	std::string pathhm = path + hmName + ".raw";
	wtr = new Writer_XML::Wtr( pathhm.c_str(), &rawHM[0], rawHM.size() );
	ok = wtr->init(true);
	delete wtr;

	std::string pathbm = path + bmName + ".map";
	wtr = new Writer_XML::Wtr( pathbm.c_str(), &rawBM[0], rawBM.size() );
	ok = wtr->init(true);
	delete wtr;

	//delete tarcook;
	delete writerXML;

	return ok;
}

void WriterMAC::uglyHeightmapToRaw( std::vector<float>& p_heightmap, float p_scale, float p_offset, std::vector<unsigned char>& io_raw ) {
	unsigned int numVertices = p_heightmap.size(); 

	io_raw.resize( numVertices );
	for( unsigned int i = 0; i < numVertices; i++ ) {
		io_raw[i] = p_heightmap[i] / p_scale - p_offset;
	}
}

void WriterMAC::uglyBlendmapToRaw(
	std::vector<Util::Texel>& p_blendmap,
	unsigned int p_row,
	unsigned int p_col,
	std::vector<unsigned char>& io_raw) {
		unsigned int numTexels = p_row * p_col;

		io_raw.resize(numTexels * 4);
		int t = 0;
		for( unsigned int i = 0; i < numTexels*4; i+=4 ) {
			int r = (int)(p_blendmap[t].m_red	* 255.0f);
			int g = (int)(p_blendmap[t].m_green	* 255.0f);
			int b = (int)(p_blendmap[t].m_blue	* 255.0f);
			int a = (int)(p_blendmap[t].m_alpha	* 255.0f);
			t++;

			std::memcpy(&io_raw[i] ,	&r,	sizeof(unsigned char));
			std::memcpy(&io_raw[i+1],	&g,	sizeof(unsigned char));
			std::memcpy(&io_raw[i+2],	&b,	sizeof(unsigned char));
			std::memcpy(&io_raw[i+3],	&a,	sizeof(unsigned char));
		}	 

		std::string temp = "";
}