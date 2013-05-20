#pragma once

#include <string>

namespace Util {
	//Nodes
	static const std::string Node_MAC		= "MAC"		;
	static const std::string Node_Resource	= "Resource";
	
	//Attributes
	static const std::string MAC_Name = "Name";

	static const std::string Resource_Type		= "Type"	;
	static const std::string Resource_Name		= "Name"	;
	static const std::string Resource_Ending	= "Ending"	;
	static const std::string Resource_CntCol	= "CntCol"	;
	static const std::string Resource_CntRow	= "CntRow"	;
	static const std::string Resource_CellSize	= "CellSize";
	static const std::string Resource_Scale		= "Scale"	;
	static const std::string Resource_Offset	= "Offset"	;

	//Expected types
	static const std::string Resource_Type_Heightmap = "Heightmap";

	struct MacHeightmap {
		std::string name;
		std::string ending;
		
		std::string cntCol;
		std::string cntRow;
		std::string cellSize;
		std::string scale;
		std::string offset;
	};
	
	struct Mac {
		std::string name;
	
		MacHeightmap heightmap;
	};
}