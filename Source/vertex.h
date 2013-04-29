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

#endif //VERTEX_H