#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <DirectXMath.h>

class MathHelper
{
public:
	static const float identityMatrix[16];

	static DirectX::XMFLOAT3 multiplyFloat3Scalar(DirectX::XMFLOAT3 vector, float scalar);
	static DirectX::XMFLOAT4X4 multiplyMatrix(DirectX::XMFLOAT4X4 matrix1, DirectX::XMFLOAT4X4 matrix2);
	
	static float lerp( float p_start, float p_end, float p_percent );
	static DirectX::XMFLOAT3 lerp( DirectX::XMFLOAT3 p_start,
		DirectX::XMFLOAT3 p_end, float p_percent );

	template<typename T>
	static void signedFastQuad( T &p_val );
	
	template<typename T>
	static T signedQuad( T p_val );
};

template<typename T>
void MathHelper::signedFastQuad( T &p_val )
{
	p_val = p_val * fabs( p_val );
}

template<typename T>
T MathHelper::signedQuad( T p_val )
{
	return p_val * fabs( p_val );
}

#endif //MATH_HELPER_H