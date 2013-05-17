#pragma once

namespace Loader_XML {
	class LoaderXML;
}

class LoaderMAC {
public:
	LoaderMAC();
	~LoaderMAC();

	bool init();
protected:
private:
	Loader_XML::LoaderXML* m_loaderXML;
};