#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <DirectXMath.h>

namespace MathHelper
{
	DirectX::XMFLOAT3 multiplyFloat3Scalar(DirectX::XMFLOAT3 vector, float scalar);
	DirectX::XMFLOAT4X4 multiplyMatrix(DirectX::XMFLOAT4X4 matrix1, DirectX::XMFLOAT4X4 matrix2);
	template<typename T>
	void signedFastQuad( T &p_val );
	template<typename T>
	T signedQuad( T p_val );
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