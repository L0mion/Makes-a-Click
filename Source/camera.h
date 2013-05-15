#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMFLOAT4X4 getViewMatrix() const;
	DirectX::XMFLOAT4X4 getProjectionMatrix() const;

	void setLens(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	void rebuildView( DirectX::XMFLOAT3 p_position,
		DirectX::XMFLOAT3 p_right, DirectX::XMFLOAT3 p_look,
		DirectX::XMFLOAT3 p_up );
private:
	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projectionMatrix;
};

#endif //CAMERA_H