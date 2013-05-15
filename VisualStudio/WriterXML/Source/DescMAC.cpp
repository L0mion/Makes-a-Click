#include "DescMAC.h"

namespace Writer_XML {
	DescMAC::DescMAC(
		std::string p_nameFile, 
		std::string p_nameLvl, 
		unsigned int p_flags ) {
			m_nameFile	= p_nameFile;
			m_nameLvl	= p_nameLvl;
			m_flags		= p_flags;
	}
	DescMAC::~DescMAC() { 
		/*Do nothing.*/ 
	}

	void DescMAC::addResource( 
		std::string p_resourceFileName,
		std::string p_resourceFileEnding,
		std::string p_resourceFileType ) {
			addResource(
				DescMACResource(
					p_resourceFileName, 
					p_resourceFileEnding, 
					p_resourceFileType));
	}

	void DescMAC::addResource( DescMACResource p_resourceDesc ) {
		m_resources.push_back(p_resourceDesc);
	}
}