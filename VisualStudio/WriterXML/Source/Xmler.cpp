#include <string>
#include <sstream>
#include <rapidxml_print.hpp>

#include <Util.h>

#include "SettingsWriterXML.h"
#include "DescMAC.h"
#include "Wtr.h"

#include "Xmler.h"

namespace Writer_XML {
	Xmler::Xmler( std::string& p_pathMac, Util::MacDesc& p_mac ) {
		m_pathMac	= &p_pathMac;
		m_mac		= &p_mac;

		m_xml = nullptr;
		m_wtr = nullptr;
	}
	Xmler::~Xmler() {
		// Not sure about what we're going to do here.
		// But it ought to be something like this:
		if( m_xml != nullptr ) {
			delete m_xml;
		}
		if( m_wtr != nullptr ) {
			delete m_wtr;
		}
	}

	void Xmler::init( std::string& io_xml ) {
		m_xml = new XmlDoc();
		XmlNode* n;
		XmlNode* p;

		//Create the original node:	
		n = allocNode( Util::Node_MAC );
		appendAtt( n, Util::Att_MAC_Version,	m_mac->version );
		appendAtt( n, Util::Att_MAC_Name,		m_mac->name );
		m_xml->append_node( n );
		p = n;

		// Link heightmap
		n = allocNode( Util::Node_Resource );
		appendAtt( n, Util::Att_Resource_Type,		Util::Resource_Type_Heightmap	);
		appendAtt( n, Util::Att_Resource_Name,		m_mac->heightmap.name			);
		appendAtt( n, Util::Att_Resource_Ending,	m_mac->heightmap.ending			);
		appendAtt( n, Util::Att_Resource_CntCol,	m_mac->heightmap.cntCol			);
		appendAtt( n, Util::Att_Resource_CntRow,	m_mac->heightmap.cntRow			);
		appendAtt( n, Util::Att_Resource_CellSize,	m_mac->heightmap.cellSize		);
		appendAtt( n, Util::Att_Resource_Scale,		m_mac->heightmap.scale			);
		appendAtt( n, Util::Att_Resource_Offset,	m_mac->heightmap.offset			);
		p->append_node( n );

		std::ostringstream oStream;
		for(unsigned  int i=0; i<m_mac->objects.size(); i++)
		{
			n = allocNode( Util::Node_Resource );
			appendAtt( n, Util::Att_Resource_Type, Util::Resource_Type_Object );
			appendAtt( n, Util::Att_Resource_Name, m_mac->objects[i].name );
			oStream.str(std::string());
			oStream << m_mac->objects[i].posX;
			appendAtt( n, Util::Att_Resource_Pos_X, oStream.str() );
			oStream.str(std::string());
			oStream << m_mac->objects[i].posY;
			appendAtt( n, Util::Att_Resource_Pos_Y, oStream.str() );
			oStream.str(std::string());
			oStream << m_mac->objects[i].posZ;
			appendAtt( n, Util::Att_Resource_Pos_Z, oStream.str() );
			p->append_node( n );
		}

		getXmlAsString( io_xml );
	}

	void Xmler::appendAtt( XmlNode* p_n, const std::string& p_a, const std::string& p_val ) {
		p_n->append_attribute( allocAtt( p_a, p_val ) );
	}

	void Xmler::getXmlAsString( std::string& p_s ) {
		rapidxml::print(
			std::back_inserter(p_s), 
			*m_xml, 
			0);
	}

	XmlNode* Xmler::allocNode( const std::string& p_s ) {
		return m_xml->allocate_node(
			rapidxml::node_element,
			allocString(p_s));
	}

	XmlAtt* Xmler::allocAtt( const std::string& p_a, const std::string& p_b ) {
		return m_xml->allocate_attribute(
			allocString(p_a),
			allocString(p_b));
	}

	char* Xmler::allocString( const std::string& p_s ) {
		return m_xml->allocate_string(p_s.c_str());
	}
}

/*
bool Xmler::print() {
std::string xml_string;
getXmlAsString(xml_string);

std::string fileName = m_mac->name;
m_wtr = new Wtr( 
*m_pathMac, 
fileName, 
xml_string );
bool sucessfulPrint = m_wtr->init();

return sucessfulPrint;
}
*/