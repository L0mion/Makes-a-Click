#pragma once

#include "DescMAC.h"

namespace Util {
	struct Mac;
}

namespace Writer_XML {
	class Xmler;
}

namespace Writer_XML {
	class WriterXML {
	public:
		WriterXML( Util::Mac& p_mac );
		~WriterXML();

		void init( std::string& io_xml );
	protected:
	private:
		Util::Mac* m_mac;
		Xmler* m_xmler;
	};
}