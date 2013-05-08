#include "constantBuffers.hlsl"
#include "structs.hlsl"
#include "lightLib.hlsl"

Texture2D texSprite : register (t0);
SamplerState ssSprite : register (s0);
 
float4 ps( VSHeightMapOut psIn ) : SV_TARGET
{
	//return float4(1,1,1,1);

	SurfaceInfo surface;
	surface.diffuse = float4( 0.5f, 0.5f, 0.5f, 0.0f );
	surface.spec = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	LightInfo light;
	light.pos		= float3( 0, 25, 0 );
	light.type 		= 0; // Should be set by instance/mesh
	light.dir 		= float3( 0, -1, 0 ); // Only used by point lights
	light.range 	= 128;
	light.att		= float3( 0, 1, 0 );
	light.spotPower = 1;
	light.ambient	= float4( 0, 0, 0 ,0 );
	light.diffuse	= float4(1,1,1,1);
	light.spec 		= float4(0,0,0,0);
	light.enabled 	= true;
	float gloss = 0;

	float3 lightOut = parallelLight( surface, light, cameraPos, psIn.normal,
		psIn.position );

	float3 color = texSprite.Sample( ssSprite, psIn.texCoord ).xyz;

	float4 finalCol = float4( color*lightOut, 1.0f ); 

	return finalCol;
}