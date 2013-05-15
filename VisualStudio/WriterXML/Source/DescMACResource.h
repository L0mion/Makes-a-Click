#pragma once

#include <string>

namespace Writer_XML {
	struct DescMACResource {
		DescMACResource(
			std::string p_resourceFileName,
			std::string p_resourceFileEnding,
			std::string p_resourceFileType) {
				resourceFileName	= p_resourceFileName;
				resourceFileEnding	= p_resourceFileEnding;
				resourceFileType	= p_resourceFileType;
		}
		~DescMACResource() { /*Do nothing.*/ }

		std::string	resourceFileName;
		std::string	resourceFileEnding;
		std::string	resourceFileType;
	};
}