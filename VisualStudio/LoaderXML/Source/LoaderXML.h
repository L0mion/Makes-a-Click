#pragma once

namespace Loader_XML {
	class WinDetective;
}
namespace Parser_XML {
	class DocXML;
}
namespace Util {
	struct Mac;
}

#include <string>

//These are fairly temporary.
static const std::wstring	g_filePathToSearch	= L"../../Resources/Levels/"	;
static const std::wstring	g_fileEnding		= L".xml"						;
static const bool			g_recursiveSearch	= true							;

/*------------------------------------------*/
/*			 - Note to self -				*/
/*				root is:					*/
/*	Makes-A-Click/VisualStudio/MakesAClick/	*/
/*------------------------------------------*/
namespace Loader_XML {
	class LoaderXML {
	public:
		LoaderXML();
		~LoaderXML();

		bool init( Util::Mac& inout_result );
	protected:
	private:
		bool parse( Util::Mac& inout_result );
		bool parseXML(
			const std::wstring filePath, 
			const std::wstring fileName, 
			const std::wstring fileEnding,
			Util::Mac& inout_result );

		Loader_XML::WinDetective* m_winDetective;
	};
}