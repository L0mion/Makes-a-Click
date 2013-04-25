
#include "Light.hlsl"

cbuffer constantBuffer
{
	float4x4 matFinal;
	float4x4 matWorld;
	float4x4 textureScale;
}

cbuffer lights
{
	Light light;
	float3 eyePosition;
}

Texture2D Texture;
SamplerState ss
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};


struct VOut
{
    float4 position : SV_POSITION;
	float3 posW		: POSITION;
	float3 normalW	: NORMAL;
    float2 texcoord	: TEXCOORD;
};

VOut VShader(float3 position : POSITION, float3 normal : NORMAL, float2 texcoord : TEXCOORD)
{
    VOut output;
    output.position = mul(matFinal, float4(position, 1.0f));
	output.posW		= mul(matWorld, float4(position, 1.0f)).xyz;
	output.normalW	= mul(matWorld, float4(normal, 0.0f)).xyz;
    output.texcoord	= mul(textureScale, float4(texcoord, 0.0f, 1.0f)).xy;

	int x = output.texcoord.x;
	output.texcoord.x -= x;
	int y = output.texcoord.y;
	output.texcoord.y -= y;

    return output;
}


float4 PShader(VOut pIn) : SV_TARGET
{
	float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	float4 color = Texture.Sample(ss, pIn.texcoord);
	float4 spec = float4(1.0f, 1.0f, 1.0f, 0.0f);
	spec.a *= 256.0f;

	float3 normalW = normalize(pIn.normalW);
	SurfaceInfo v = {pIn.posW, pIn.normalW, diffuse, spec};

	float3 litColor = PointLight(v, light, eyePosition);
	//float3 litColor = ParallelLight(light, v, eyePosition);

	if(litColor.x < 0.1f)
	{
		litColor = float3(0.1f, 0.1f, 0.1f);
	}

	//return color;
	return color*float4(litColor, diffuse.a);
	//return float4(litColor, diffuse.a);
}