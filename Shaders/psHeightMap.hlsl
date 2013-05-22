#include "constantBuffers.hlsl"
#include "structs.hlsl"
#include "lightLib.hlsl"

Texture2D texSand	: register (t0);
Texture2D texStone	: register (t1);
Texture2D texGrass	: register (t2);
Texture2D texGravel : register (t3);
Texture2D texBlendMap : register (t4);
SamplerState ssSprite : register (s0);
 
float4 ps( VSHeightMapOut psIn ) : SV_TARGET
{
	//return float4(1,1,1,1);

	//psIn.normal = normalize(psIn.normal);

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

	float3 lightOut = parallelLight( surface, light, cameraPos,
		psIn.normal.xyz, psIn.position.xyz );

	float3 colorSand	= texSand.Sample( ssSprite, psIn.texCoord ).xyz;
	float3 colorStone	= texStone.Sample(ssSprite, psIn.texCoord).xyz;
	float3 colorGrass	= texGrass.Sample(ssSprite, psIn.texCoord).xyz;
	float3 colorGravel	= texGravel.Sample(ssSprite, psIn.texCoord).xyz;
	
	float2 unTiledTex = float2(psIn.texCoord.x/255.0f, psIn.texCoord.y/255.0f); 
	float4 blendFactor = texBlendMap.Sample(ssSprite, unTiledTex);

	float3 color = colorSand;
	color = lerp(color, colorStone, blendFactor.x);
	color = lerp(color, colorGrass, blendFactor.y);
	color = lerp(color, colorGravel, blendFactor.z);

	float4 finalCol = float4( color*lightOut, 1.0f ); 

	//return psIn.normal;
	return finalCol;
}