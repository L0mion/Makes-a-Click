#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <DirectXMath.h>

namespace MathHelper
{
	DirectX::XMFLOAT3 multiplyFloat3Scalar(DirectX::XMFLOAT3 vector, float scalar);
	DirectX::XMFLOAT3 transformNormalFloat3(DirectX::XMFLOAT3 vector, float angle);
};

#endif //MATH_HELPER_H