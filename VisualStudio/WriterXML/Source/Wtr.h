#pragma once

#include <fstream>

namespace Writer_XML {
	enum WtrStatuses {
		WtrStatuses_OK, 
		WtrStatuses_NOT_FOUND, 
		WtrStatuses_EOF, 
		WtrStatuses_BAD, 
		WtrStatuses_FAIL
	};

	class Wtr { //Writer! :D
	public:
		Wtr(
			const char* p_filename,
			const unsigned char* p_data,
			unsigned long p_dataCnt);
		~Wtr();

		bool init( bool p_binary );
		void wt(); //Write! :D
	protected:
	private:
		void getWtrStatus( WtrStatuses& p_wtrStatus );

		const char* m_filename;
		const unsigned char* m_data;
		unsigned long m_dataCnt;

		//std::string* m_fileName;
		//std::string* m_filePath;
		//std::string* m_toWrite;
		
		std::ofstream* m_outputStream;
		WtrStatuses m_wtrStatus;
	};
}