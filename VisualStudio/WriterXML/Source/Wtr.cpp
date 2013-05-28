#include "Wtr.h"

namespace Writer_XML {
	Wtr::Wtr( 
		const char* p_filename,
		const char* p_data,
		unsigned long p_dataCnt) : m_filename(p_filename), m_data(p_data) {
		m_dataCnt = p_dataCnt;

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

	bool Wtr::init( bool p_binary ) {
		bool sucessfulWt = false;
		
		m_outputStream = new std::ofstream();
		if( p_binary==true ) {
			m_outputStream->open(
				m_filename, 
				std::ios::out | std::ios::binary );
		} else {
			m_outputStream->open (
				m_filename, 
				std::ios::out );
		}
		
		m_wtrStatus = WtrStatuses_OK;
		getWtrStatus( m_wtrStatus );
		if( m_wtrStatus == WtrStatuses_OK ) {
			wt();
			m_outputStream->close();
			sucessfulWt = true;
		}

		return sucessfulWt;
	}

	void Wtr::wt() {
		m_outputStream->write( m_data, m_dataCnt );
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