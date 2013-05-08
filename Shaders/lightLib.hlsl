struct LightInfo
{
	float4x4	lightViewProj;
	float3		pos;
	int			type;
	float3		dir;
	float		range;
	float3		att;
	float		spotPower;
	float4		ambient;
	float4		diffuse;
	float4		spec;
	int			enabled;
	float3		padding;
};

struct SurfaceInfo
{
	float4 diffuse;
	float4 spec;
};

float3 parallelLight(SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
 
	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -light.dir;

	// Add the ambient term.
	litColor += surface.diffuse * light.ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower  = max(surface.spec.a, 1.0f);
		float3 toEye     = normalize(eyePos - pos);
		float3 R         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * surface.diffuse * light.diffuse;
		litColor += specFactor * surface.spec * light.spec;
	}

	return litColor;
}

float3 pointLight(SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = light.pos - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	if( d > light.range )
		return float3(0.0f, 0.0f, 0.0f);

	// Normalize the light vector.
	lightVec /= d; 

	// Add the ambient light term.
	litColor += surface.diffuse * light.ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower  = max(surface.spec.a, 1.0f);
		float3 toEye     = normalize(eyePos - pos);
		float3 R         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * surface.diffuse * light.diffuse.rgb;
		litColor += specFactor * surface.spec * light.spec;
	}

	// attenuate
	return litColor / dot(light.att, float3(1.0f, d, d*d));
}

float3 spotLight(SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pos)
{
	float3 litColor = pointLight(surface, light, eyePos, normal, pos);

	// The vector from the surface to the light.
	float3 lightVec = normalize(light.pos - pos);

	float s = pow(max(dot(-lightVec, light.dir), 0.0f), light.spotPower);

	// Scale color by spotlight factor.
	return litColor*s;
}

// Used to calculate shadow map offset used when sampling from the shadow map
float2 texOffset( int u, int v )
{
	float2 shadowMapSize = float2(2000, 2000); //HACK! hardcoded values
	return float2( u * 1.0f/shadowMapSize.x, v * 1.0f/shadowMapSize.y );
}

//float shadowFactor( float4 lpos, Texture2D tex)
//{
//	//re-homogenize position after interpolation
//	lpos.xyz /= lpos.w;
//	// lpos *= -1; // DEBUG
//
//	//if position is not visible to the light - dont illuminate it
//	//results in hard light frustum
//	if( lpos.x < -1.0f || lpos.x > 1.0f ||
//		lpos.y < -1.0f || lpos.y > 1.0f ||
//		lpos.z < 0.0f  || lpos.z > 1.0f )
//	{
//		return -1.0f;
//	}
//
//	//transform clip space coords to texture space coords (-1:1 to 0:1)
//	lpos.x = lpos.x/2 + 0.5;
//	lpos.y = lpos.y/-2 + 0.5;
//
//	//sample shadow map - point sampler
//	float shadowMapDepth = tex.Sample(pointSampler, lpos.xy).r;
//	//return float4(shadowMapDepth, shadowMapDepth, shadowMapDepth, 1.0f); // DEBUG
//
//	//if clip space z value greater than shadow map value then pixel is in shadow
//	float e = 0.0003f;
//	lpos.z -= e;
//
//	//PCF sampling for shadow map
//	float sum = 0;
//	float x, y;
//
//	//perform PCF filtering on a 4 x 4 texel neighborhood
//	for (y = -1.5; y <= 1.5; y += 1.0)
//	{
//		for (x = -1.5; x <= 1.5; x += 1.0)
//		{
//			sum += tex.SampleCmpLevelZero( 
//				cmpSampler, lpos.xy + texOffset(x,y), lpos.z );
//		}
//	}
//	float shadowFactor = sum / 16.0;
//	//if ( shadowMapDepth + e < lpos.z)
//	//	return float4(0.0f, 1.0f, 0.0f, 1.0f);<
//
//	return shadowFactor;
//}