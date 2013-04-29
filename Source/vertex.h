#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

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

struct SpriteVertex
{
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT2 m_texCoords;

	SpriteVertex()
	{
		m_position  = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_texCoords = DirectX::XMFLOAT2(0.0f, 0.0f);
	}
	SpriteVertex(DirectX::XMFLOAT3 p_position, DirectX::XMFLOAT2 p_texCoords)
	{
		m_position = p_position;
		m_texCoords  = p_texCoords;
	}
};

#endif //VERTEX_H