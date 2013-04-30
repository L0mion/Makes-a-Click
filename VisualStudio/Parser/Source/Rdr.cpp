#include "Rdr.h"

Rdr::Rdr(std::string p_tgtFilename) {
	m_tgtFilename = p_tgtFilename;

	m_inputStream = nullptr;
}
Rdr::~Rdr() {
	if(m_inputStream)
		delete m_inputStream;
}

Rdr::RdStatuses Rdr::init() {
	m_inputStream = new std::ifstream(m_tgtFilename);
	Rdr::RdStatuses readStatus = rd();

	return readStatus;
}

Rdr::RdStatuses Rdr::rd() {
	Rdr::RdStatuses rdStatus = getRdStatus();
	if( rdStatus == RdStatuses_OK ) {
		//This reads the entire file into a string. This includes formatting, such as \n etc.
		std::string rdText(
			(std::istreambuf_iterator<char>(*m_inputStream)), 
			std::istreambuf_iterator<char>());
		m_rdTxt = rdText;

		//This might be some alternative way of doing some stuff. Saving it for brevity.
		//unsigned int inputCnt = static_cast<unsigned int>(m_inputStream->gcount());
		//Read entire file into character array and store.
		//char* readTxt = new char[inputCnt]; //Ought to have a check to see whether or not we can override memory available on the stack.
		//m_inputStream->read(readTxt, inputCnt);
		//m_rdTxt = std::string(readTxt);
		//
		//delete[] readTxt; //No. No. I clean.
	}

	return rdStatus;
}

Rdr::RdStatuses Rdr::getRdStatus() {
	Rdr::RdStatuses rdStatus = RdStatuses_OK;
	
	if( !m_inputStream->is_open() )		{
		rdStatus = RdStatuses_NOT_FOUND;
	} else if( m_inputStream->eof() )	{
		rdStatus = RdStatuses_EOF;
	} else if( m_inputStream->bad() )	{
		rdStatus = RdStatuses_BAD;
	} else if( m_inputStream->fail() )	{
		rdStatus = RdStatuses_FAIL;
	}

	return rdStatus;
}