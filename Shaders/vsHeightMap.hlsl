#include "constantBuffers.hlsl"
#include "structs.hlsl"

VSHeightMapOut vs( VSHeightMapIn vsIn )
{
	VSHeightMapOut output;
	output.position = mul( float4(vsIn.position, 1.0f), finalMatrix );
	//output.normal = mul( float4(vsIn.normal, 0.0f), finalMatrix );
	output.normal = float4( vsIn.normal, 0.0f); 
	output.normal = normalize( output.normal );
	output.texCoord = vsIn.texCoord;

	return output;
}