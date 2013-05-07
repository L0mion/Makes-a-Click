#pragma once

class WinDetective;

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

	WinDetective* m_winDetective;
};