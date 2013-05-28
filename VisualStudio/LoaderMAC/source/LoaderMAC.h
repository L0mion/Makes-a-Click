#pragma once

class LoaderHeightmap;
class LoaderRaw;

namespace Loader_XML {
	class LoaderXML;
}

namespace Util {
	struct MacDesc;
}

#include <string>
#include <vector>
#include <Util.h>

#include "Mac.h"

class LoaderMAC {
public:
	LoaderMAC();
	~LoaderMAC();

	bool init( Mac& io_result );
protected:
private:
	void uglyRawToBlendmap( std::vector<unsigned char>& p_raw, unsigned int p_row, unsigned int p_col, std::vector<Util::Texel>& io_bm );

	const std::wstring	m_filePathToSearch;
	const std::wstring	m_fileEnding;
	const bool			m_recursiveSearch;

	//Xml
	Loader_XML::LoaderXML* m_loaderXML;
	
	//Heightmap
	LoaderHeightmap* m_loaderHM;

	//Blendmap
	LoaderRaw* m_loaderBM;
};