#pragma once

namespace Writer_XML {
	class DescMAC;
	class Xmler;
}

namespace Writer_XML {
	class WriterXML {
	public:
		WriterXML( DescMAC& p_descMac );
		~WriterXML();

		void init();
	protected:
	private:
		DescMAC* m_descMac;
		Xmler* m_xmler;
	};
}