#pragma once

namespace Parser_XML {
	class DocXML;
}

#include <string>

namespace Parser_XML {
	class ParserXML {
	public:
		ParserXML(char* p_xml);
		~ParserXML();

		bool init();

		void getDocXML(DocXML** inout_docXML);
	protected:
	private:
		bool parse(std::string p_txtToParse);

		char* m_xml;

		Parser_XML::DocXML* m_docXML;
		bool m_memManagement; //Whether mem. responsibility lie with ParserXML.
	};
}