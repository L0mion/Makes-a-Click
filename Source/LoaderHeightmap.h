#pragma once

class LoaderRaw;

#include <string>
#include <vector>

class LoaderHeightmap {
public:
	LoaderHeightmap(
		std::string p_rawPath,
		unsigned int p_cntRow,
		unsigned int p_cntCol,
		float p_scale,
		float p_offset );
	~LoaderHeightmap();

	bool init( std::vector<float>& io_heightmap );
protected:
private:
	void scaleHeightmap( std::vector<float>& io_heightmap, std::vector<unsigned char>& p_raw );
	void smoothHeightmap( std::vector<float>& io_heightmap );
	float average( 
		std::vector<float>& io_heightmap,
		unsigned int p_indexRow,
		unsigned int p_indexCol);
	bool inBounds( unsigned int i, unsigned int j );

	std::string m_rawPath;
	unsigned int m_cntRow;
	unsigned int m_cntCol;
	float m_scale;
	float m_offset;

	LoaderRaw* m_loaderRaw;
};