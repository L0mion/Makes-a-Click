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
			std::string& p_fileName,
			std::string& p_filePath,
			std::string& p_toWrite );
		~Wtr();

		bool init();
		void wt(); //Write! :D
	protected:
	private:
		void getWtrStatus( WtrStatuses& p_wtrStatus );

		std::string* m_fileName;
		std::string* m_filePath;
		std::string* m_toWrite;
		
		std::ofstream* m_outputStream;
		WtrStatuses m_wtrStatus;
	};
}