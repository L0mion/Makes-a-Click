#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <DirectXMath.h>

namespace MathHelper
{
	DirectX::XMFLOAT3 multiplyFloat3Scalar(DirectX::XMFLOAT3 vector, float scalar);
	DirectX::XMFLOAT4X4 multiplyMatrix(DirectX::XMFLOAT4X4 matrix1, DirectX::XMFLOAT4X4 matrix2);
};

#endif //MATH_HELPER_H