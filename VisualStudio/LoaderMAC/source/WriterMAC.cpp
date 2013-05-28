#include <WriterXML.h>
#include <Wtr.h>
#include <stdlib.h>
#include <cstdlib>

#include <Tartar.h> //awwwyeah

#include "Mac.h"

#include "WriterMAC.h"

WriterMAC::WriterMAC( Mac& io_mac ) {
	m_mac = &io_mac;
}
WriterMAC::~WriterMAC() {
	// Do nothing.
}

bool WriterMAC::init() {
	Util::Mac macmac = m_mac->macDesc;
	
	// Get xml
	std::string xml;
	Writer_XML::WriterXML* writerXML = new Writer_XML::WriterXML( macmac );
	writerXML->init( xml );
	
	// Get raw
	std::vector<char> raw;
	uglyHeightmapToRaw( 
		m_mac->heightmap, 
		(float)std::atof(macmac.heightmap.scale.c_str()),  //HAHA
		(float)std::atof(macmac.heightmap.offset.c_str()), //HAHA 
		raw );

	std::string xmlName = m_mac->macDesc.name + ".xml";
	std::string rawName = macmac.heightmap.name;

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
	Writer_XML::Wtr* wtr = new Writer_XML::Wtr( pathxml.c_str(), xml.c_str(), xml.size() );
	bool ok = wtr->init(true);
	delete wtr;

	std::string pathraw = path + rawName + ".raw";
	wtr = new Writer_XML::Wtr( pathraw.c_str(), &raw[0], raw.size() );
	ok = wtr->init(true);
	delete wtr;

	//delete tarcook;
	delete writerXML;

	return ok;
}

void WriterMAC::uglyHeightmapToRaw( std::vector<float>& p_heightmap, float p_scale, float p_offset, std::vector<char>& io_raw ) {
	unsigned int numVertices = p_heightmap.size(); 

	io_raw.resize( numVertices );
	for( unsigned int i = 0; i < numVertices; i++ ) {
		io_raw[i] = p_heightmap[i] / p_scale - p_offset;
	}
}