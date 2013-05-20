#pragma once

class LoaderHeightmap;

namespace Loader_XML {
	class LoaderXML;
}

namespace Util {
	struct Mac;
}

#include <string>

class LoaderMAC {
public:
	LoaderMAC();
	~LoaderMAC();

	bool init();
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