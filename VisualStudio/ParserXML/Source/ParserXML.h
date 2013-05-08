#pragma once

namespace Parser_XML {
	class DocXML;
}

#include <string>

namespace Parser_XML {
	class ParserXML {
	public:
		ParserXML(std::string p_filePath, std::string p_fileName);
		~ParserXML();

		bool init();

		void getDocXML(DocXML** inout_docXML);
	protected:
	private:
		bool parse(std::string p_txtToParse);

		std::string m_filePath;
		std::string m_fileName;
		std::string m_filePathFull;

		Parser_XML::DocXML* m_docXML;
		bool m_memManagement; //Whether mem. responsibility lie with ParserXML.
	};
}