#pragma once

class LoaderHeightmap;

namespace Loader_XML {
	class LoaderXML;
}

namespace Util {
	struct Mac;
}

#include <string>
#include <vector>

#include "Mac.h"

class LoaderMAC {
public:
	LoaderMAC();
	~LoaderMAC();

	bool init( Mac& io_result );
protected:
private:
	const std::wstring	m_filePathToSearch;
	const std::wstring	m_fileEnding;
	const bool			m_recursiveSearch;

	//Xml
	Loader_XML::LoaderXML* m_loaderXML;
	
	//Heightmap
	LoaderHeightmap* m_loaderHM;
};