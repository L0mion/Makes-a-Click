#pragma once

#include "DescMAC.h"

namespace Util {
	struct MacDesc;
}

namespace Writer_XML {
	class Xmler;
}

namespace Writer_XML {
	class WriterXML {
	public:
		WriterXML( Util::MacDesc& p_mac );
		~WriterXML();

		void init( std::string& io_xml );
	protected:
	private:
		Util::MacDesc* m_mac;
		Xmler* m_xmler;
	};
}