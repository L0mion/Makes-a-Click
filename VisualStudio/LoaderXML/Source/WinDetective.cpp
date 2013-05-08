#include <windows.h>

#include "WinDetective.h"

namespace Loader_XML {
	WinDetective::WinDetective( 
		std::wstring p_filePathStart, 
		std::wstring p_suspectFileEnding, 
		bool p_recursiveSearch ) {
			m_filePathStart			= p_filePathStart;
			m_suspectFileEnding		= p_suspectFileEnding;
			m_recursiveSearch		= p_recursiveSearch;

			////Convert to wstring.
			//UtilString::Std2W(m_filePathStartW,		m_filePathStartStd);
			//UtilString::Std2W(m_suspectFileEndingW,	m_suspectFileEndingStd);
			//
			////Convert to LPCSTR.
			//m_filePathStartLPC		= m_filePathStartStd.c_str();
			//m_suspectFileEndingLPC	= m_suspectFileEndingStd.c_str();
	}
	WinDetective::~WinDetective() {
		//Do nothing.
	}

	bool WinDetective::init() {
		if(m_recursiveSearch) {
			investigateFolders();
		}
		investigateFiles();

		bool foundCulprits = false;
		if(m_locatedCulprits.size()) {
			foundCulprits = true;
		}
		return foundCulprits;
	}

	void WinDetective::hireInvestigator(
		std::wstring p_filePathStart, 
		std::wstring p_suspectFileEnding,
		bool p_recursiveSearch ) {
			//Create a new investigator and search the specified folder recursively.
			WinDetective winDetective(
				p_filePathStart, 
				p_suspectFileEnding, 
				p_recursiveSearch);
			bool foundCulprits = winDetective.init();
			if( foundCulprits ) {
				std::vector<WinDetective::Culprit> locatedCulprits = winDetective.getLocatedCulprits();
				for(unsigned int i = 0; i < locatedCulprits.size(); i++) {
					m_locatedCulprits.push_back(locatedCulprits.at(i));
				}
			}
	}

	void WinDetective::investigateFolders() {
		//We start of by recursively locating files in folders in order to build our hierarchy.
		//File we're looking for, e.g. * - everything.
		std::wstring suspectPhantomPic(L"*");

		//Folder in which we start looking for suspects.
		std::wstring curCrimeSceneW		= m_filePathStart;
		std::wstring curCrimeSceneDescW	= m_filePathStart;
		curCrimeSceneDescW.append(suspectPhantomPic);

		// FindFirstFile wants a LPCTSTR, which is defined as const wchar_t*
		//  depending on _UNICODE.
		WIN32_FIND_DATA findFileData;
		HANDLE searchHandleWinAPI = FindFirstFile(
			curCrimeSceneDescW.c_str(),	//_In_   LPCTSTR			lpFileName,
			&findFileData);				//_Out_  LPWIN32_FIND_DATA	lpFindFileData
		if(searchHandleWinAPI != INVALID_HANDLE_VALUE) {
			do {
				if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					//If not a folder, iterate to next.
					if( wcscmp((wchar_t*)findFileData.cFileName, L".") == 0 
						|| wcscmp((wchar_t*)findFileData.cFileName, L"..") == 0 ) {
							continue;
					}
					//Found folder, establish path into folder:
					std::wstring curPath = curCrimeSceneW;
					std::wstring cFileName((wchar_t*)findFileData.cFileName);
					curPath.append(cFileName);
					curPath.append(L"/");

					//Hire another investigator to investigate new crime scene.
					hireInvestigator(
						curPath, 
						m_suspectFileEnding,
						m_recursiveSearch );
				}
			} while(FindNextFile(searchHandleWinAPI, &findFileData));
			FindClose(searchHandleWinAPI);
		}
	}
	void WinDetective::investigateFiles() {
		std::wstring crimeSceneW = m_filePathStart;
		crimeSceneW.append(L"*");
		crimeSceneW.append(m_suspectFileEnding);

		WIN32_FIND_DATA findFileData;
		HANDLE searchHandleWinAPI = FindFirstFile(crimeSceneW.c_str(), &findFileData);
		if(searchHandleWinAPI != INVALID_HANDLE_VALUE) {
			do {
				std::wstring culpritFileName((wchar_t*)findFileData.cFileName);
				Culprit c(m_filePathStart, culpritFileName, m_suspectFileEnding);
				m_locatedCulprits.push_back(c);
			} while(FindNextFile(searchHandleWinAPI, &findFileData));
			FindClose(searchHandleWinAPI);
		}
	}
}

//USE ME
//DWORD lastError = GetLastError();