#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMFLOAT3 getPosition() const;
	DirectX::XMFLOAT3 getRight() const;
	DirectX::XMFLOAT3 getLook() const;
	DirectX::XMFLOAT3 getUp() const;

	DirectX::XMFLOAT4X4 getViewMatrix() const;
	DirectX::XMFLOAT4X4 getProjectionMatrix() const;

	void setLens(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	void strafe(float distance);
	void walk(float distance);
	void verticalWalk(float distance);

	void pitch(float angle);
	void yaw(float angle);

	void rebuildView();
private:
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT3 right_;
	DirectX::XMFLOAT3 look_;
	DirectX::XMFLOAT3 up_;

	DirectX::XMFLOAT4X4 viewMatrix_;
	DirectX::XMFLOAT4X4 projectionMatrix_;
};

#endif //CAMERA_H