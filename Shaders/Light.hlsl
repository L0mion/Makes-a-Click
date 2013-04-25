
struct Light
{
	float3 pos;
	float3 dir;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att;
	float  spotPower;
	float  range;
};

struct SurfaceInfo
{
	float3 pos;
    float3 normal;
    float4 diffuse;
    float4 spec;
};

float3 ParallelLight(Light L, SurfaceInfo v, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	
	float3 lightVec = -L.dir;

	float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 temp = L.ambient * v.diffuse;
	litColor += temp.xyz;

	float diffuseFactor = dot(lightVec, v.normal);
	if(diffuseFactor > 0.0f)
	{
		float specPower = max(v.spec.a, 1.0f);
		float3 toEye = normalize(eyePos-v.pos);
		float3 R = reflect(-lightVec, v.normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		//Diffuse and specular terms

		temp = diffuseFactor * v.diffuse * L.diffuse;
		litColor += temp.xyz;
		temp = specFactor * v.spec * L.spec;
		litColor += temp.xyz;
	}

	return litColor;
}

float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	//Temp is used as a middle stage in calculations with float4 vectors
	//where the answer must fit in a float3 vector.
	float4 temp;

	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	
	// The vector from the surface to the light.
	float3 lightVec = L.pos - v.pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	if( d > L.range )
		return float3(0.0f, 0.0f, 0.0f);
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Add the ambient light term.
	temp =  v.diffuse * L.ambient;
	litColor +=	temp.xyz;
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower  = max(v.spec.a, 1.0f);
		float3 toEye     = normalize(eyePos - v.pos);
		float3 R         = reflect(-lightVec, v.normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
		
		// diffuse and specular terms
		temp = diffuseFactor * v.diffuse * L.diffuse;
		litColor += temp.xyz;
		
		temp = v.spec * L.spec;
		temp = temp * specFactor;
		litColor += temp.xyz;
	}
	
	// attenuate
	return litColor / dot(L.att, float3(1.0f, d, d*d));
}


//float3 Spotlight(SurfaceInfo v, Light L, float3 eyePos)
//{
//	float3 litColor = PointLight(v, L, eyePos);
//	
//	// The vector from the surface to the light.
//	float3 lightVec = normalize(L.pos - v.pos);
//	
//	float s = pow(max(dot(-lightVec, L.dir), 0.0f), L.spotPower);
//	
//	// Scale color by spotlight factor.
//	return litColor*s;
//}