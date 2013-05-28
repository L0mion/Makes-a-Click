#include <Util.h>

#include "InterpreterXML.h"

namespace Loader_XML {
	InterpreterXML::InterpreterXML( rapidxml::xml_document<>* p_toInterpret ) {
		m_toInterpret = p_toInterpret;
	}
	InterpreterXML::~InterpreterXML() {
		//Do nothing.
	}

	bool InterpreterXML::init( Util::Mac& io_result ) {
		bool sucessfulInterpret = false;

		rapidxml::xml_node<>* curNode = m_toInterpret->first_node( Util::Node_MAC.c_str() );
		if( curNode!=NULL ) {
			//Get MAC-attributes
			std::string macName;
			sucessfulInterpret = getAttVal( curNode, Util::Att_MAC_Name, macName );
			if( sucessfulInterpret==true ) {
				io_result.name = macName;

				//Proceed to get resource-data:
				sucessfulInterpret = interpretResource( io_result, curNode );
			}
		}

		return sucessfulInterpret;
	}

	bool InterpreterXML::interpretResource( Util::Mac& io_result, rapidxml::xml_node<>* p_curNode ) {
		bool validResource = nextNode( &p_curNode, Util::Node_Resource );
		if( validResource==true ) {
			//Get MAC-attributes:
			std::string resourceType;
			validResource = getAttVal( p_curNode, Util::Att_Resource_Type, resourceType );
			if( validResource==true ) {
				//Depending on resource-type, parse the correct whatevs.
				if( resourceType==Util::Resource_Type_Heightmap ) {
					interpretHeightmap( io_result, p_curNode );
				} //else if( resourceType==Util::Resource_Type_Texture... )
			}
		}

		return validResource;
	}

	bool InterpreterXML::interpretHeightmap( Util::Mac& io_result, rapidxml::xml_node<>* p_curNode ) {
		//Name
		std::string val;
		bool validHeightMap = getAttVal( p_curNode, Util::Att_Resource_Name, val );
		if( validHeightMap==true ) {
			io_result.heightmap.name = val;
		}

		//Ending
		validHeightMap = validHeightMap && getAttVal( p_curNode, Util::Att_Resource_Ending, val );
		if( validHeightMap==true ) {
			io_result.heightmap.ending = val;
		}

		//CntCol
		validHeightMap = validHeightMap && getAttVal( p_curNode, Util::Att_Resource_CntCol, val );
		if( validHeightMap==true ) {
			io_result.heightmap.cntCol = val;
		}

		//CntRow
		validHeightMap = validHeightMap && getAttVal( p_curNode, Util::Att_Resource_CntRow, val );
		if( validHeightMap==true ) {
			io_result.heightmap.cntRow = val;
		}

		//CellSize
		validHeightMap = validHeightMap && getAttVal( p_curNode, Util::Att_Resource_CellSize, val );
		if( validHeightMap==true ) {
			io_result.heightmap.cellSize = val;
		}

		//Scale
		validHeightMap = validHeightMap && getAttVal( p_curNode, Util::Att_Resource_Scale, val );
		if( validHeightMap==true ) {
			io_result.heightmap.scale = val;
		}

		//Offset
		validHeightMap = validHeightMap && getAttVal( p_curNode, Util::Att_Resource_Offset, val );
		if( validHeightMap==true ) {
			io_result.heightmap.offset = val;
		}

		return validHeightMap;
	}

	bool InterpreterXML::getAttVal( 
		rapidxml::xml_node<>*	p_curNode,
		const std::string&		p_targetAtt,
		std::string&			io_val) {
			rapidxml::xml_attribute<>* att = nullptr;
			bool validAtt = getAtt( p_curNode, p_targetAtt, &att );
			if( validAtt==true ) {
				io_val = att->value(); //(*att)->value();
			}
			
			return validAtt;
	}

	bool InterpreterXML::nextNode( rapidxml::xml_node<>** p_curNode, const std::string& p_nextNode ) {
		bool validNode = false;

		rapidxml::xml_node<>* newNode = (*p_curNode)->first_node( p_nextNode.c_str() );
		if( newNode!=NULL ) {
			*p_curNode = newNode;
			validNode = true;
		}
		return validNode;
	}
	bool InterpreterXML::getAtt(
		rapidxml::xml_node<>*		p_curNode, 
		const std::string&			p_targetAtt, 
		rapidxml::xml_attribute<>**	io_att ) {
			bool validAtt = false;

			rapidxml::xml_attribute<>* newAtt = p_curNode->first_attribute( p_targetAtt.c_str() );
			if( newAtt!=NULL ) {
				*io_att = newAtt;
				validAtt = true;
			}
			return validAtt;
	}
}