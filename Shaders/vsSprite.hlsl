#include "constantBuffers.hlsl"
#include "structs.hlsl"

VSSpriteOut vsSprite(VSSpriteIn vsIn)
{
	VSSpriteOut output;
	output.position = mul(float4(vsIn.position, 1.0f), spriteTransform);
	output.texCoords = vsIn.texCoords;

	return output;
}