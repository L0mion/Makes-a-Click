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
		LoaderXML( char* p_xml );
		~LoaderXML();

		bool init( Util::MacDesc& inout_result );
	protected:
	private:
		char* m_xml;
	};
}