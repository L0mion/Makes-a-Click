#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

struct Coords 
{
	enum{ X, Y, Z, XYZ_CNT };
	enum{ U, V, UV_CNT };
};

struct Vertex
{
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT4 color_;

	Vertex()
	{
		position_	= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		color_		= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	Vertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color)
	{
		position_	= position;
		color_		= color;
	}
};

struct HeightMapVertex
{
	float normal[Coords::XYZ_CNT];
	float position[Coords::XYZ_CNT];
	float texCoord[Coords::UV_CNT];

	HeightMapVertex();
	HeightMapVertex( float p_posX,	float p_posY,	float p_posZ,
		float p_normX,	float p_normY,	float p_normZ, 
		float p_texU,	float p_texV );
};


#endif //VERTEX_H