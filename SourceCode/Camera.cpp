#include "Camera.h"


Camera::Camera()
{
	position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{
}

D3DXVECTOR3 Camera::getPosition() const
{
	return position;
}

D3DXVECTOR3 Camera::getRight() const
{
	return right;
}

D3DXVECTOR3 Camera::getLook() const
{
	return look;
}

D3DXVECTOR3 Camera::getUp() const
{
	return up;
}

D3DXMATRIX Camera::getView() const
{
	return view;
}

D3DXMATRIX Camera::getProjection() const
{
	return projection;
}

void Camera::setLens(float fov, float aspect, float zn, float zf)
{
	D3DXMATRIX perspective;
	ZeroMemory(&perspective, sizeof(D3DXMATRIX));

	perspective._11 = 1/(aspect * (tan(fov/2)));
	perspective._22 = 1/(tan(fov/2));
	perspective._33 = zf/(zf-zn);
	perspective._34 = 1.0f;
	perspective._43 = (-zn*zf)/(zf-zn);

	projection = perspective;
}

void Camera::strafe(float velocity)
{
	position += velocity*right;
}

void Camera::walk(float velocity)
{
	position += velocity*look;
}

void Camera::verticalWalk(float velocity)
{
	position.y += velocity;
}

void Camera::setHeight(float height)
{
	position.y = height;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &right, angle);

	D3DXVec3TransformNormal(&up, &up, &rotation);
	D3DXVec3TransformNormal(&look, &look, &rotation);
}

void Camera::rotateY(float angle)
{
	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, angle);

	D3DXVec3TransformNormal(&right, &right, &rotation);
	D3DXVec3TransformNormal(&up, &up, &rotation);
	D3DXVec3TransformNormal(&look, &look, &rotation);
}

void Camera::rebuildView()
{
	D3DXVec3Normalize(&look, &look);

	D3DXVec3Cross(&up, &look, &right);
	D3DXVec3Normalize(&up, &up);

	D3DXVec3Cross(&right, &up, &look);
	D3DXVec3Normalize(&right, &right);

	float x = -D3DXVec3Dot(&position, &right);
	float y = -D3DXVec3Dot(&position, &up);
	float z = -D3DXVec3Dot(&position, &look);

	view(0,0) = right.x; 
	view(1,0) = right.y; 
	view(2,0) = right.z; 
	view(3,0) = x;   

	view(0,1) = up.x;
	view(1,1) = up.y;
	view(2,1) = up.z;
	view(3,1) = y;  

	view(0,2) = look.x; 
	view(1,2) = look.y; 
	view(2,2) = look.z; 
	view(3,2) = z;   

	view(0,3) = 0.0f;
	view(1,3) = 0.0f;
	view(2,3) = 0.0f;
	view(3,3) = 1.0f;
}