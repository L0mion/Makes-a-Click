#ifndef STRUCTS_HLSL
#define STRUCTS_HLSL

struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VSOut
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};

struct VSHeightMapIn
{
	float3 normal	: NORMAL;
	float3 position : POSITION;
	float2 texCoord	: TEXCOORD;
};

struct VSHeightMapOut
{
	float4 normal	: NORMAL;
	float4 position : SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

struct VSSpriteIn
{
	float3 position  : POSITION;
	float2 texCoords : TEXCOORD;
};
struct VSSpriteOut
{
	float4 position  : SV_POSITION;
	float2 texCoords : TEXCOORD; 
};

#endif //STRUCTS_HLSL