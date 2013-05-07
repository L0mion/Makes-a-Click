#include "constantBuffers.hlsl"
#include "structs.hlsl"

Texture2D texSprite : register (t0);
SamplerState ssSprite : register (s0);
 
float4 ps( VSHeightMapOut psIn ) : SV_TARGET
{
	return float4(1,1,1,1);

	float4 color = texSprite.Sample( ssSprite, psIn.texCoord );

	return color;
}