#include "camera.h"
#include "mathHelper.h"

Camera::Camera()
{
	float identityMatrix[] = {  1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f};

	m_viewMatrix		= DirectX::XMFLOAT4X4( identityMatrix );
	m_projectionMatrix	= DirectX::XMFLOAT4X4( identityMatrix );
}

Camera::~Camera()
{
}

DirectX::XMFLOAT4X4 Camera::getViewMatrix() const
{
	return m_viewMatrix;
}
DirectX::XMFLOAT4X4 Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

void Camera::setLens(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	float zeroMatrix[16] = {0.0f};
	m_projectionMatrix = DirectX::XMFLOAT4X4(zeroMatrix);

	m_projectionMatrix._11 = 1/(aspectRatio * (tan(fieldOfView/2)));
	m_projectionMatrix._22 = 1/(tan(fieldOfView/2));
	m_projectionMatrix._33 = farPlane/(farPlane-nearPlane);
	m_projectionMatrix._34 = 1.0f;
	m_projectionMatrix._43 = (-nearPlane*farPlane)/(farPlane-nearPlane);
}

void Camera::rebuildView( DirectX::XMFLOAT3 p_position,
	DirectX::XMFLOAT3 p_right, DirectX::XMFLOAT3 p_look, 
	DirectX::XMFLOAT3 p_up )
{
	DirectX::XMVECTOR vLook	= DirectX::XMLoadFloat3(&p_look);
	DirectX::XMVECTOR vUp	= DirectX::XMLoadFloat3(&p_up);
	DirectX::XMVECTOR vRight	= DirectX::XMLoadFloat3(&p_right);
	
	vLook = DirectX::XMVector3Normalize(vLook);
	
	vUp = DirectX::XMVector3Cross(vLook, vRight);
	vUp = DirectX::XMVector3Normalize(vUp);
	
	vRight = DirectX::XMVector3Cross(vUp, vLook);
	vRight = DirectX::XMVector3Normalize(vRight);
	
	DirectX::XMVECTOR vPosition = DirectX::XMVectorSet( p_position.x*-1.0f, p_position.y*-1.0f, p_position.z*-1.0f, 1.0f);
	
	DirectX::XMVECTOR vResult;
	DirectX::XMFLOAT3 fResult;
	vResult = DirectX::XMVector3Dot(vPosition, vRight);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float x = fResult.x;
	vResult = DirectX::XMVector3Dot(vPosition, vUp);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float y = fResult.y;
	vResult = DirectX::XMVector3Dot(vPosition, vLook);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float z = fResult.z;
	
	DirectX::XMStoreFloat3(&p_right, vRight);
	DirectX::XMStoreFloat3(&p_up, vUp);
	DirectX::XMStoreFloat3(&p_look, vLook);
	
	m_viewMatrix(0, 0) = p_right.x;
	m_viewMatrix(1, 0) = p_right.y;
	m_viewMatrix(2, 0) = p_right.z;
	m_viewMatrix(3, 0) = x;
		
	m_viewMatrix(0, 1) = p_up.x;
	m_viewMatrix(1, 1) = p_up.y;
	m_viewMatrix(2, 1) = p_up.z;
	m_viewMatrix(3, 1) = y;
		
	m_viewMatrix(0, 2) = p_look.x;
	m_viewMatrix(1, 2) = p_look.y;
	m_viewMatrix(2, 2) = p_look.z;
	m_viewMatrix(3, 2) = z;
		
	m_viewMatrix(0, 3) = 0.0f;
	m_viewMatrix(1, 3) = 0.0f;
	m_viewMatrix(2, 3) = 0.0f;
	m_viewMatrix(3, 3) = 1.0f;
}