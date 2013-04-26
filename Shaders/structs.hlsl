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
	float4 color : COLOR;
};

#endif //STRUCTS_HLSL