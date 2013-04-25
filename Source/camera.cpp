#include "camera.h"
#include "mathHelper.h"

Camera::Camera()
{
	position_ = DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f);
	right_	  = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	look_	  = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	up_		  = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	float identityMatrix[] = {  1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f};

	viewMatrix_		  = DirectX::XMFLOAT4X4(identityMatrix);
	projectionMatrix_ = DirectX::XMFLOAT4X4(identityMatrix);
}

Camera::~Camera()
{
}

DirectX::XMFLOAT3 Camera::getPosition() const
{
	return position_;
}
DirectX::XMFLOAT3 Camera::getRight() const
{
	return right_;
}
DirectX::XMFLOAT3 Camera::getLook() const
{
	return look_;
}
DirectX::XMFLOAT3 Camera::getUp() const
{
	return up_;
}

DirectX::XMFLOAT4X4 Camera::getViewMatrix() const
{
	return viewMatrix_;
}
DirectX::XMFLOAT4X4 Camera::getProjectionMatrix() const
{
	return projectionMatrix_;
}

void Camera::setLens(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	float zeroMatrix[16] = {0.0f};
	projectionMatrix_ = DirectX::XMFLOAT4X4(zeroMatrix);

	projectionMatrix_._11 = 1/(aspectRatio * (tan(fieldOfView/2)));
	projectionMatrix_._22 = 1/(tan(fieldOfView/2));
	projectionMatrix_._33 = farPlane/(farPlane-nearPlane);
	projectionMatrix_._34 = 1.0f;
	projectionMatrix_._43 = (-nearPlane*farPlane)/(farPlane-nearPlane);
}

void Camera::strafe(float distance)
{
	position_.x += right_.x * distance;
	position_.y += right_.y * distance;
	position_.z += right_.z * distance;
}
void Camera::walk(float distance)
{
	position_.x += look_.x * distance;
	position_.y += look_.y * distance;
	position_.z += look_.z * distance;
}
void Camera::verticalWalk(float distance)
{
	position_.y += distance;
}

void Camera::pitch(float angle)
{
	DirectX::XMMATRIX rotationMatrix;
	DirectX::XMVECTOR xmRight	= DirectX::XMLoadFloat3(&right_);
	DirectX::XMVECTOR xmLook	= DirectX::XMLoadFloat3(&look_);
	DirectX::XMVECTOR xmUp		= DirectX::XMLoadFloat3(&up_);
	rotationMatrix = DirectX::XMMatrixRotationAxis(xmRight, angle);

	xmLook	= DirectX::XMVector3TransformNormal(xmLook, rotationMatrix);
	xmUp	= DirectX::XMVector3TransformNormal(xmUp, rotationMatrix);

	DirectX::XMStoreFloat3(&look_, xmLook);
	DirectX::XMStoreFloat3(&up_, xmUp);
}
void Camera::yaw(float angle)
{
	DirectX::XMVECTOR yAxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX rotationMatrix;

	DirectX::XMVECTOR xmRight	= DirectX::XMLoadFloat3(&right_);
	DirectX::XMVECTOR xmLook	= DirectX::XMLoadFloat3(&look_);
	DirectX::XMVECTOR xmUp		= DirectX::XMLoadFloat3(&up_);

	rotationMatrix = DirectX::XMMatrixRotationAxis(yAxis, angle);
	xmRight	= DirectX::XMVector3TransformNormal(xmRight, rotationMatrix);
	xmLook	= DirectX::XMVector3TransformNormal(xmLook, rotationMatrix);
	xmUp	= DirectX::XMVector3TransformNormal(xmUp, rotationMatrix);

	DirectX::XMStoreFloat3(&right_, xmRight);
	DirectX::XMStoreFloat3(&look_, xmLook);
	DirectX::XMStoreFloat3(&up_, xmUp);
}

void Camera::rebuildView()
{
	DirectX::XMVECTOR vLook	= DirectX::XMLoadFloat3(&look_);
	DirectX::XMVECTOR vUp	= DirectX::XMLoadFloat3(&up_);
	DirectX::XMVECTOR vRight	= DirectX::XMLoadFloat3(&right_);
	
	vLook = DirectX::XMVector3Normalize(vLook);
	
	vUp = DirectX::XMVector3Cross(vLook, vRight);
	vUp = DirectX::XMVector3Normalize(vUp);
	
	vRight = DirectX::XMVector3Cross(vUp, vLook);
	vRight = DirectX::XMVector3Normalize(vRight);
	
	DirectX::XMVECTOR vPosition = DirectX::XMVectorSet( position_.x*-1.0f, position_.y*-1.0f, position_.z*-1.0f, 1.0f);
	
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
	
	DirectX::XMStoreFloat3(&right_, vRight);
	DirectX::XMStoreFloat3(&up_, vUp);
	DirectX::XMStoreFloat3(&look_, vLook);
	
	viewMatrix_(0, 0) = right_.x;
	viewMatrix_(1, 0) = right_.y;
	viewMatrix_(2, 0) = right_.z;
	viewMatrix_(3, 0) = x;
		
	viewMatrix_(0, 1) = up_.x;
	viewMatrix_(1, 1) = up_.y;
	viewMatrix_(2, 1) = up_.z;
	viewMatrix_(3, 1) = y;
		
	viewMatrix_(0, 2) = look_.x;
	viewMatrix_(1, 2) = look_.y;
	viewMatrix_(2, 2) = look_.z;
	viewMatrix_(3, 2) = z;
		
	viewMatrix_(0, 3) = 0.0f;
	viewMatrix_(1, 3) = 0.0f;
	viewMatrix_(2, 3) = 0.0f;
	viewMatrix_(3, 3) = 1.0f;
}