#pragma once

#include "SettingsParserXML.h"

namespace Parser_XML {
	class DocXML;
}

namespace Parser_XML {
	class RdrXML {
	public:
		RdrXML(DocXML* inout_docXML);
		~RdrXML();

		bool init();
	protected:
	private:
		Parser_XML::DocXML* m_inout_docXML;
	};
}