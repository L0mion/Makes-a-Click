#include <Util.h>

#include "InterpreterXML.h"

namespace Loader_XML {
	InterpreterXML::InterpreterXML( rapidxml::xml_document<>* p_toInterpret ) {
		m_toInterpret = p_toInterpret;
	}
	InterpreterXML::~InterpreterXML() {
		//Do nothing.
	}

	bool InterpreterXML::init( Util::MacDesc& io_result ) {
		bool sucessfulInterpret = false;

		rapidxml::xml_node<>* curNode = m_toInterpret->first_node( Util::Node_MAC.c_str() );
		if( curNode!=NULL ) {
			//Get MAC-attributes
			std::string macName;
			sucessfulInterpret = getAttVal( curNode, Util::Att_MAC_Name, macName );
			if( sucessfulInterpret==true ) {
				io_result.name = macName;

				// Resources
				rapidxml::xml_node<>* node = curNode;
				for(rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
					interpretResource( io_result, child );
				}
			}
		}

		return sucessfulInterpret;
	}

	bool InterpreterXML::interpretResource( Util::MacDesc& io_result, rapidxml::xml_node<>* p_curNode ) {
		//bool validResource = nextNode( &p_curNode, Util::Node_Resource );
		std::string resourceType;
		bool validResource = getAttVal( p_curNode, Util::Att_Resource_Type, resourceType );
		if( validResource==true ) {
			//Depending on resource-type, parse the correct whatevs.
			if( resourceType==Util::Resource_Type_Heightmap ) {
				interpretHeightmap( io_result, p_curNode );
			} else if( resourceType==Util::Resource_Type_Blendmap ) {
				interpretBlendmap( io_result, p_curNode );
			} else if( resourceType==Util::Resource_Type_Object ) {
				interpretObject( io_result, p_curNode );
			}
		}

		return validResource;
	}

	bool InterpreterXML::interpretHeightmap( Util::MacDesc& io_result, rapidxml::xml_node<>* p_curNode ) {
		//Name
		std::string val;
		bool validHM = getAttVal( p_curNode, Util::Att_Resource_Name, val );
		if( validHM==true ) {
			io_result.heightmap.name = val;
		}

		//Ending
		validHM = validHM && getAttVal( p_curNode, Util::Att_Resource_Ending, val );
		if( validHM==true ) {
			io_result.heightmap.ending = val;
		}

		//CntCol
		validHM = validHM && getAttVal( p_curNode, Util::Att_Resource_CntCol, val );
		if( validHM==true ) {
			io_result.heightmap.cntCol = val;
		}

		//CntRow
		validHM = validHM && getAttVal( p_curNode, Util::Att_Resource_CntRow, val );
		if( validHM==true ) {
			io_result.heightmap.cntRow = val;
		}

		//CellSize
		validHM = validHM && getAttVal( p_curNode, Util::Att_Resource_CellSize, val );
		if( validHM==true ) {
			io_result.heightmap.cellSize = val;
		}

		//Scale
		validHM = validHM && getAttVal( p_curNode, Util::Att_Resource_Scale, val );
		if( validHM==true ) {
			io_result.heightmap.scale = val;
		}

		//Offset
		validHM = validHM && getAttVal( p_curNode, Util::Att_Resource_Offset, val );
		if( validHM==true ) {
			io_result.heightmap.offset = val;
		}

		return validHM;
	}

	bool InterpreterXML::interpretBlendmap(	Util::MacDesc& io_result, rapidxml::xml_node<>* p_curNode ) {
		//Name
		std::string val;
		bool validBM = getAttVal( p_curNode, Util::Att_Resource_Name, val );
		if( validBM==true ) {
			io_result.blendmap.name = val;
		}

		//Ending
		validBM = validBM && getAttVal( p_curNode, Util::Att_Resource_Ending, val );
		if( validBM==true ) {
			io_result.blendmap.ending = val;
		}

		//CntCol
		validBM = validBM && getAttVal( p_curNode, Util::Att_Resource_CntCol, val );
		if( validBM==true ) {
			io_result.blendmap.height = val;
		}

		//CntRow
		validBM = validBM && getAttVal( p_curNode, Util::Att_Resource_CntRow, val );
		if( validBM==true ) {
			io_result.blendmap.width = val;
		}

		return validBM;
	}

	bool InterpreterXML::interpretObject( Util::MacDesc& io_result, rapidxml::xml_node<>* p_curNode )
	{
		io_result.objects.push_back(Util::MacObject());
		std::string val;
		bool validObject = getAttVal( p_curNode, Util::Att_Resource_Name, val );
		if( validObject ) {
			io_result.objects.back().name = val;
		}

		validObject = validObject && getAttVal( p_curNode, Util::Att_Resource_Pos_X, val );
		if( validObject ) {
			io_result.objects.back().posX = ::atof(val.c_str());
		}

		validObject = validObject && getAttVal( p_curNode, Util::Att_Resource_Pos_Y, val );
		if( validObject ) {
			io_result.objects.back().posY = ::atof(val.c_str());
		}

		validObject = validObject && getAttVal( p_curNode, Util::Att_Resource_Pos_Z, val );
		if( validObject ) {
			io_result.objects.back().posZ = ::atof(val.c_str());
		}

		return validObject;
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