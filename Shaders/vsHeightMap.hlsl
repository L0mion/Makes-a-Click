#include "constantBuffers.hlsl"
#include "structs.hlsl"

VSHeightMapOut vs( VSHeightMapIn vsIn )
{
	VSHeightMapOut output;

	float4x4 final = mul( world, finalMatrix );

	output.position = mul( float4(vsIn.position, 1.0f), final );
	output.normal = float4( vsIn.normal, 0.0f); 
	output.normal = normalize( output.normal );
	output.texCoord = vsIn.texCoord;

	return output;
}