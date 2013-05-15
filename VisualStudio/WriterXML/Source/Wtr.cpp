#include "Wtr.h"

namespace Writer_XML {
	Wtr::Wtr( 
		std::string& p_fileName,
		std::string& p_filePath,
		std::string& p_toWrite ) {
		m_fileName	= &p_fileName;
		m_filePath	= &p_filePath;
		m_toWrite	= &p_toWrite;

		m_outputStream = nullptr;
	}
	Wtr::~Wtr() {
		if( m_outputStream != nullptr ) {
			if( m_outputStream->is_open() ) {
				m_outputStream->close();
			}
			delete m_outputStream;
		}
	}

	bool Wtr::init() {
		bool sucessfulWt = false;

		std::string fullPath = *m_filePath;
		fullPath += *m_fileName;
		
		m_outputStream = new std::ofstream();
		m_outputStream->open (
			fullPath, 
			std::ios::out );
		
		m_wtrStatus = WtrStatuses_OK;
		getWtrStatus( m_wtrStatus );
		if( m_wtrStatus == WtrStatuses_OK ) {
			wt();
			sucessfulWt = true;
		}

		return sucessfulWt;
	}

	void Wtr::wt() {
		*m_outputStream << m_toWrite;
	}

	void Wtr::getWtrStatus( WtrStatuses& p_wtrStatus ) {
		if( !m_outputStream->is_open() )		{
			p_wtrStatus = WtrStatuses_NOT_FOUND;
		} else if( m_outputStream->eof() )	{
			p_wtrStatus = WtrStatuses_EOF;
		} else if( m_outputStream->bad() )	{
			p_wtrStatus = WtrStatuses_BAD;
		} else if( m_outputStream->fail() )	{
			p_wtrStatus = WtrStatuses_FAIL;
		}
	}
}