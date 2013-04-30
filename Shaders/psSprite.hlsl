#include "constantBuffers.hlsl"
#include "structs.hlsl"

Texture2D texSprite : register (t0);
SamplerState ssSprite : register (s0);
 
float4 psSprite(VSSpriteOut psIn) : SV_TARGET
{
	float4 color = texSprite.Sample(ssSprite, psIn.texCoords);

	return color;
}