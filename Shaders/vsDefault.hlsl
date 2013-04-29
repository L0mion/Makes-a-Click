
#include "constantBuffers.hlsl"
#include "structs.hlsl"

VSOut vsDefault(VSIn vsIn)
{
	VSOut vsOut;

	vsOut.position = mul(float4(vsIn.position, 1.0f), finalMatrix);
	vsOut.color = vsIn.color;

	return vsOut;
}