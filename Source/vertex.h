#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

struct Coords 
{
	enum{ X, Y, Z, XYZ_CNT };
	enum{ U, V, UV_CNT };
};

struct Vertex_CP
{
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT4 color_;

	Vertex_CP()
	{
		position_	= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		color_		= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	Vertex_CP(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color)
	{
		position_	= position;
		color_		= color;
	}
};

struct Vertex_PT
{
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT2 m_texCoords;

	Vertex_PT()
	{
		m_position  = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_texCoords = DirectX::XMFLOAT2(0.0f, 0.0f);
	}
	Vertex_PT(DirectX::XMFLOAT3 p_position, DirectX::XMFLOAT2 p_texCoords)
	{
		m_position = p_position;
		m_texCoords  = p_texCoords;
	}
};

struct Vertex_PNT
{
	float position[Coords::XYZ_CNT];
	float normal[Coords::XYZ_CNT];
	float texCoord[Coords::UV_CNT];

	Vertex_PNT();
	Vertex_PNT( 
		float p_posX,	float p_posY,	float p_posZ,
		float p_normX,	float p_normY,	float p_normZ, 
		float p_texU,	float p_texV );
};

#endif //VERTEX_H