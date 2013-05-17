#include "LoaderRaw.h"

#include "LoaderHeightmap.h"

LoaderHeightmap::LoaderHeightmap(
	std::string p_rawPath,
	unsigned int p_cntRow,
	unsigned int p_cntCol,
	float p_scale,
	float p_offset ) {
	m_rawPath	= p_rawPath;
	m_cntRow	= p_cntRow;
	m_cntCol	= p_cntCol;
	m_scale		= p_scale;
	m_offset	= p_offset;

	m_loaderRaw = nullptr;
}
LoaderHeightmap::~LoaderHeightmap() {
	if( m_loaderRaw != nullptr ) {
		delete m_loaderRaw;
	}
}

bool LoaderHeightmap::init( std::vector<float>& io_heightmap ) {
	std::vector<unsigned char> raw;
	m_loaderRaw = new LoaderRaw( m_rawPath, m_cntRow, m_cntCol );
	bool sucessfulLoad = m_loaderRaw->init(raw);

	if( sucessfulLoad == true ) {
		scaleHeightmap( io_heightmap, raw );
	}
}

void LoaderHeightmap::scaleHeightmap( std::vector<float>& io_heightmap, std::vector<unsigned char>& p_raw ) {
	unsigned int numVertices = m_cntRow * m_cntCol;
	
	io_heightmap.resize( p_raw.size() );
	for( unsigned int i = 0; i < numVertices; i++ ) {
		io_heightmap[i] = p_raw[i] * m_scale + m_offset;
	}
}

void LoaderHeightmap::smoothHeightmap( std::vector<float>& io_heightmap ) {
	std::vector<float> temp( io_heightmap.size() );
	for( unsigned int indexRow = 0; indexRow < m_cntRow; indexRow++ ) {
		for( unsigned int indexCol = 0; indexCol < m_cntCol; indexCol++ ) {
			temp[ m_cntCol * indexRow + indexCol ] = average(io_heightmap, indexRow, indexCol );
		}
	}

	io_heightmap = temp;
}

float LoaderHeightmap::average( std::vector<float>& io_heightmap, unsigned int p_indexRow, unsigned int p_indexCol ) {
	float avg = 0.0f;
	float num = 0.0f;

	for( int indexRow = p_indexRow - 1; indexRow <= p_indexRow + 1; indexRow++ ) {
		for( int indexCol = p_indexCol - 1; indexCol <= p_indexCol + 1; indexCol++ ) {
			if(inBounds( indexRow, indexCol )) {
				avg += io_heightmap[ indexRow * m_cntCol + indexCol ];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

bool LoaderHeightmap::inBounds( unsigned int i, unsigned int j ) {
	return 
		i >= 0			&& 
		i < m_cntRow	&&
		j >= 0			&& 
		j < m_cntCol;
}