
RWTexture2D<float4> output : register(u0);

[numthreads(32, 32, 1)]
void csPrimaryRayStage( uint3 threadID : SV_DispatchThreadID, uint groupID : SV_GroupID )
{		
	output[threadID.xy] = float4(0.0f, 0.0f, 0.7f, 1.0f);
}