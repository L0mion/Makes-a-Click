#pragma once

#include <string>
#include <fstream>

class Rdr {
public:
	enum RdStatuses { RdStatuses_OK, RdStatuses_NOT_FOUND, RdStatuses_EOF, RdStatuses_BAD, RdStatuses_FAIL };

	Rdr(std::string p_tgtFilename);
	~Rdr();

	Rdr::RdStatuses init();

	std::string getRdTxt() const { return m_rdTxt; }
protected:
private:
	RdStatuses rd();
	RdStatuses getRdStatus();

	std::string m_tgtFilename;
	std::string m_rdTxt;

	std::ifstream* m_inputStream;
};