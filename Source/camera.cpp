#include "camera.h"
#include "mathHelper.h"

Camera::Camera()
{
	position_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	right_	  = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	look_	  = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
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
}