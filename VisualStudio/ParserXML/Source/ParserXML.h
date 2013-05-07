#pragma once

#include <string>

class ParserXML {
public:
	ParserXML(std::string filePath, std::string fileName);
	~ParserXML();

	bool init();
protected:
private:
	void parse(std::string txtToParse);

	std::string m_filePath;
	std::string m_fileName;
	std::string m_filePathFull;
};