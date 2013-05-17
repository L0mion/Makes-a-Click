#include "LoaderRaw.h"

LoaderRaw::LoaderRaw(std::string p_filePath, unsigned int p_cntRow, unsigned int p_cntCol) {
	m_filePath = p_filePath;
	m_cntRow = p_cntRow;
	m_cntCol = p_cntCol;

	m_ifstream = nullptr;
}
LoaderRaw::~LoaderRaw() {
	if( m_ifstream != nullptr ) {
		if( m_ifstream->is_open() ) {
			m_ifstream->close();
		}
		delete m_ifstream;
	}
}

bool LoaderRaw::init( std::vector<unsigned char>& inout_raw ) {
	bool sucessfulLoad = false;
	
	unsigned int numVertices = m_cntRow * m_cntCol;
	inout_raw.resize( numVertices );
	
	m_ifstream = new std::ifstream();
	m_ifstream->open( m_filePath, std::ios_base::binary );

	if( m_ifstream->is_open() ) {
		m_ifstream->read( (char*)&inout_raw[0], inout_raw.size() );
		m_ifstream->close();

		sucessfulLoad = true;
	}

	return sucessfulLoad;
}