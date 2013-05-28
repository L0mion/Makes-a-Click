#pragma once

#include <rapidxml.hpp>

#include <string>

namespace Util {
	struct MacDesc;
}

namespace Loader_XML {
	class InterpreterXML {
	public:
		InterpreterXML( rapidxml::xml_document<>* p_toInterpret );
		~InterpreterXML();
	
		bool init( Util::MacDesc& io_result );
	protected:
	private:
		bool interpretResource(		Util::MacDesc& io_result, rapidxml::xml_node<>* p_curNode );
		bool interpretHeightmap(	Util::MacDesc& io_result, rapidxml::xml_node<>* p_curNode );
		bool interpretObject(		Util::MacDesc& io_result, rapidxml::xml_node<>* p_curNode );
		bool interpretBlendmap(		Util::MacDesc& io_result, rapidxml::xml_node<>* p_curNode );


		bool getAttVal( 
			rapidxml::xml_node<>*	p_curNode,
			const std::string&		p_targetAtt,
			std::string&			io_val);

		bool nextNode(
			rapidxml::xml_node<>**	p_curNode,
			const std::string&		p_nextNode );
		bool getAtt(
			rapidxml::xml_node<>*		p_curNode, 
			const std::string&			p_targetAtt, 
			rapidxml::xml_attribute<>**	io_att );

		rapidxml::xml_document<>* m_toInterpret;
	};
}