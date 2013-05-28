#pragma once

namespace Loader_XML {
	class WinDetective;
}
namespace Parser_XML {
	class DocXML;
}
namespace Util {
	struct MacDesc;
}

#include <string>

/*------------------------------------------*/
/*			 - Note to self -				*/
/*				root is:					*/
/*	Makes-A-Click/VisualStudio/MakesAClick/	*/
/*------------------------------------------*/
namespace Loader_XML {
	class LoaderXML {
	public:
		LoaderXML(
			std::wstring	p_filePathToSearch,
			std::wstring	p_fileEnding,
			bool			p_recursiveSearch);
		~LoaderXML();

		bool init( Util::MacDesc& inout_result );
	protected:
	private:
		bool parse( Util::MacDesc& inout_result );
		bool parseXML(
			const std::wstring filePath, 
			const std::wstring fileName, 
			const std::wstring fileEnding,
			Util::MacDesc& inout_result );

		std::wstring m_filePathToSearch;
		std::wstring m_fileEnding;
		bool m_recursiveSearch;

		Loader_XML::WinDetective* m_winDetective;
	};
}