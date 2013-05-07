#include "Vertex.h"


HeightMapVertex::HeightMapVertex()
{
	// Set default values which are easily identifiable
	position[Coords::X]	= 1.23f;
	position[Coords::Y]	= 2.34f;
	position[Coords::Z]	= 3.45f;
	normal[Coords::X]	= 4.56f;
	normal[Coords::Y]	= 5.67f;
	normal[Coords::Z]	= 6.78f;
	texCoord[Coords::U]	= 7.89f;
	texCoord[Coords::V]	= 8.90f;
}

HeightMapVertex::HeightMapVertex( float p_posX, float p_posY, float p_posZ, float p_normX, float p_normY, float p_normZ, float p_texU, float p_texV )
{
	position[Coords::X]	= p_posX;
	position[Coords::Y]	= p_posY;
	position[Coords::Z]	= p_posZ;
	normal[Coords::X]	= p_normX;
	normal[Coords::Y]	= p_normY;
	normal[Coords::Z]	= p_normZ;
	texCoord[Coords::U]	= p_texU;
	texCoord[Coords::V]	= p_texV;
}
