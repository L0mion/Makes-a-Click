#include "mathHelper.h"

const float MathHelper::identityMatrix[16] = 
{   1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

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

float MathHelper::lerp( float p_start, float p_end, float p_percent )
{
	return (p_start + p_percent*(p_end - p_start));
}

DirectX::XMFLOAT3 MathHelper::lerp( DirectX::XMFLOAT3 p_start,
	DirectX::XMFLOAT3 p_end, float p_percent )
{
	DirectX::XMFLOAT3 tmp;
	tmp.x = p_start.x + p_percent*(p_end.x - p_start.x);
	tmp.y = p_start.y + p_percent*(p_end.y - p_start.y);
	tmp.z = p_start.z + p_percent*(p_end.z - p_start.z);

	return tmp;
}
