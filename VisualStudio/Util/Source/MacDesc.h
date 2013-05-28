#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace Util {
	//Nodes
	static const std::string Node_MAC			= "MAC"		;
	static const std::string Att_MAC_Version	= "Version"	;
	static const std::string Att_MAC_Name		= "Name"	;

	static const std::string Node_Resource			= "Resource";
	static const std::string Att_Resource_Type		= "Type"	;
	static const std::string Att_Resource_Name		= "Name"	;
	static const std::string Att_Resource_Ending	= "Ending"	;
	static const std::string Att_Resource_CntCol	= "CntCol"	;
	static const std::string Att_Resource_CntRow	= "CntRow"	;
	static const std::string Att_Resource_CellSize	= "CellSize";
	static const std::string Att_Resource_Scale		= "Scale"	;
	static const std::string Att_Resource_Offset	= "Offset"	;
	static const std::string Att_Resource_Pos_X		= "PosX"	;
	static const std::string Att_Resource_Pos_Y		= "PosY"	;
	static const std::string Att_Resource_Pos_Z		= "PosZ"	;

	//Expected types
	static const std::string Resource_Type_Heightmap	= "Heightmap";
	static const std::string Resource_Type_Blendmap		= "Blendmap";

	static const std::string Resource_Type_Object = "Object";
	static const std::string Object_Type_Barrel = "Barrel";

	struct MacHeightmap {
		std::string name;
		std::string ending;
		
		std::string cntCol;
		std::string cntRow;
		std::string cellSize;
		std::string scale;
		std::string offset;
	};

	struct MacBlendmap {
		std::string name;
		std::string ending;
		std::string width;
		std::string height;
	};

	struct MacObject {
		std::string name;
		float posX;
		float posY;
		float posZ;
	};
	
	struct MacDesc {
		std::string name;
		std::string version;
	
		// Resources
		MacHeightmap heightmap;
		MacBlendmap blendmap;

		std::vector<MacObject> objects;
	};
}