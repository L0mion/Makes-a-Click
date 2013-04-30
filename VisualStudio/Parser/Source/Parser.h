#pragma once

#include <string>

class Parser {
public:
	Parser(std::string filePath, std::string fileName);
	~Parser();

	bool init();
protected:
private:
	void parse(std::string txtToParse);

	std::string m_filePath;
	std::string m_fileName;
	std::string m_filePathFull;
};