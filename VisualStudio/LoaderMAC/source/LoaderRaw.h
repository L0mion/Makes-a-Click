#pragma once

#include <vector>
#include <fstream>

class LoaderRaw {
public:
	LoaderRaw(
		std::string p_filePath,
		unsigned int p_cntRow, 
		unsigned int p_cntCol);
	~LoaderRaw();

	bool init( std::vector<unsigned char>& inout_raw );
protected:
private:
	std::string m_filePath;
	unsigned int m_cntRow;
	unsigned int m_cntCol;

	std::ifstream* m_ifstream;
};