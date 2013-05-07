#pragma once

typedef const char* LPCSTR;

#include <string>
#include <vector>

class WinDetective {
public:
	struct Culprit {
		std::wstring filePath;
		std::wstring fileName;
		std::wstring fileEnding;

		Culprit(
			std::wstring filePath, 
			std::wstring fileName, 
			std::wstring fileEnding ) {
			this->filePath		= filePath;
			this->fileName		= fileName;
			this->fileEnding	= fileEnding;
		}
		~Culprit() { /*Do nothing.*/ }
	};

	WinDetective(
		std::wstring p_filePathStart,
		std::wstring p_suspectFileEnding,
		bool p_recursiveSearch );
	~WinDetective();

	bool init();

	std::vector<Culprit> getLocatedCulprits() { return m_locatedCulprits; }
protected:
private:
	void hireInvestigator(
		std::wstring p_filePathStart, 
		std::wstring p_suspectFileEnding,
		bool p_recursiveSearch );
	void investigateFolders();
	void investigateFiles();

	std::wstring	m_filePathStart;
	std::wstring	m_suspectFileEnding;
	bool m_recursiveSearch;

	//Attributes
	//std::string	m_filePathStartStd;
	//std::string	m_suspectFileEndingStd;
	//bool m_recursiveSearch;
	//
	//std::wstring m_filePathStartW;		//wstring (technically std too, but hey.)
	//std::wstring m_suspectFileEndingW;	//..
	//LPCSTR m_filePathStartLPC;			//LPCSTR
	//LPCSTR m_suspectFileEndingLPC;		//..

	std::vector<Culprit> m_locatedCulprits;
};

/*
/////////\\\\\\\\\
// WIN-DETECTIVE \\
\\\\\\\\\//////////
          .=.,                                              
         ;c =\                                              
       __|  _/
     .'-'-._/-'-._                                          
    /..   ____    \      
   /' _  [<_->] )  \                                        
  (  / \--\_>/-/'._ )                                       
   \-;_/\__;__/ _/ _/                                       
    '._}|==o==\{_\/                                         
     /  /-._.--\  \_                                        
    // /   /|   \ \ \                                       
   / | |   | \;  |  \ \                                     
  / /  | :/   \: \   \_\                                    
 /  |  /.'|   /: |    \ \                                   
 |  |  |--| . |--|     \_\                                  
 / _/   \ | : | /___--._) \                                 
|_(---'-| >-'-| |       '-'                                 
       /_/     \_\
*/