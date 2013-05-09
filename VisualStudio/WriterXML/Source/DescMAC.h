#pragma once

#include <string>
#include <vector>

#include "DescMACResource.h"

namespace Writer_XML {
	class DescMAC {
	public:
		enum DescMACFlags {
			DescMACFlags_HEIGHTMAP		= 0x01,	/**< Heightmap. */
			//DescMACFlags_BLENDMAP		= 0x02, /**< Possible settings... */
			//DescMACFlags_SOMESETTING	= 0x04,	/**< Possible settings... */
			//0x08, 0x10, 0x20
		};

		DescMAC(
			std::string p_nameFile, 
			std::string p_nameLvl, 
			unsigned int p_flags );
		~DescMAC();

		void addResource( 
			std::string p_resourceFileName,
			std::string p_resourceFileEnding,
			std::string p_resourceFileType );

		std::string		getNameFile()	const { return m_nameFile;	}
		std::string		getNameLvl()	const { return m_nameLvl;	}
		unsigned int	getFlags()		const { return m_flags;		}
	protected:
	private:
		void addResource( DescMACResource p_resourceDesc );

		//Attributes:
		std::string m_nameFile;
		std::string m_nameLvl;
		unsigned int m_flags;

		std::vector<DescMACResource> m_resources;
	};
}