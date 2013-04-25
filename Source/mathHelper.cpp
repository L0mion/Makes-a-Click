#include "mathHelper.h"

DirectX::XMFLOAT3 MathHelper::multiplyFloat3Scalar(DirectX::XMFLOAT3 vector, float scalar)
{
	DirectX::XMVECTOR xmVector = DirectX::XMLoadFloat3(&vector);
	xmVector = DirectX::XMVectorScale(xmVector, scalar);
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, xmVector);
	return result;
}