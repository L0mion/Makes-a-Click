#include "mathHelper.h"

DirectX::XMFLOAT3 MathHelper::multiplyFloat3Scalar(DirectX::XMFLOAT3 vector, float scalar)
{
	DirectX::XMVECTOR xmVector = DirectX::XMLoadFloat3(&vector);
	xmVector = DirectX::XMVectorScale(xmVector, scalar);
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, xmVector);
	return result;
}

DirectX::XMFLOAT4X4 MathHelper::multiplyMatrix(DirectX::XMFLOAT4X4 matrix1, DirectX::XMFLOAT4X4 matrix2)
{
	DirectX::XMFLOAT4X4 result;
	DirectX::XMMATRIX xm1 = DirectX::XMLoadFloat4x4(&matrix1);
	DirectX::XMMATRIX xm2 = DirectX::XMLoadFloat4x4(&matrix2);
	DirectX::XMMATRIX xmResult = DirectX::XMMatrixMultiply(xm1, xm2);
	DirectX::XMStoreFloat4x4(&result, xmResult);

	return result;
}