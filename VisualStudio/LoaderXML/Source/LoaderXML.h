#pragma once

namespace Loader_XML {
	class WinDetective;
}
namespace Parser_XML {
	class DocXML;
}

#include <string>

//These are fairly temporary.
static const std::wstring	g_filePathToSearch	= L"../../Resources/"	;
static const std::wstring	g_fileEnding		= L".xml"				;
static const bool			g_recursiveSearch	= true					;

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

		bool init();
	protected:
	private:
		bool parse();
		bool parseXML(
			const std::wstring filePath, 
			const std::wstring fileName, 
			const std::wstring fileEnding);
		void loadXML(Parser_XML::DocXML* docXML);

		Loader_XML::WinDetective* m_winDetective;
	};
}