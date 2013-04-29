#include "Rdr.h"

Rdr::Rdr(std::string p_tgtFilename) {
	m_tgtFilename = p_tgtFilename;

	m_inputStream = nullptr;
}
Rdr::~Rdr() {
	if(m_inputStream)
		delete m_inputStream;
}

bool Rdr::init() {
	bool sucessfulParse = false;

	m_inputStream = new std::ifstream(m_tgtFilename);
	if(m_inputStream->is_open()) {
		unsigned int inputCnt = static_cast<unsigned int>(m_inputStream->gcount());

		//Read entire file into character array and store.
		char* readTxt = new char[inputCnt]; //Ought to have a check to see whether or not we can override memory available on the stack.
		m_inputStream->read(readTxt, inputCnt);
		m_rdTxt = std::string(readTxt);

		delete[] readTxt; //No. No. I clean.
		sucessfulParse = true;
	}

	return sucessfulParse;
}