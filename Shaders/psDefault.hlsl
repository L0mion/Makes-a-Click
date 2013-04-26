
#include "constantBuffers.hlsl"
#include "structs.hlsl"

float4 psDefault(VSOut psIn) : SV_TARGET
{
	return psIn.color;
}