#ifndef CONSTANT_BUFFERS_HLSL
#define CONSTANT_BUFFERS_HLSL

#pragma pack_matrix(row_major)

cbuffer cbFrame : register (b0)
{
	row_major float4x4 finalMatrix;
	float3 cameraPos;
	float pad;
};

cbuffer cbSprite : register (b1)
{
	row_major float4x4 spriteTransform;
};

cbuffer cbSprite : register (b2)
{
	row_major float4x4 world;
};

#endif //CONSTANT_BUFFERS_HLSL