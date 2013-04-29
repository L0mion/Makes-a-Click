#pragma once

#include <string>
#include <fstream>

class Rdr
{
public:
	Rdr(std::string p_tgtFilename);
	~Rdr();

	bool init();

	std::string getRdTxt() const { return m_rdTxt; }
protected:
private:
	std::string m_tgtFilename;
	std::string m_rdTxt;

	std::ifstream* m_inputStream;
};