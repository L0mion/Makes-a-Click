#include "sprite.h"

Sprite::Sprite()
{
	m_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale    = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_textureId  = TextureIds::TextureIds_PLACEHOLDER; 
}
Sprite::Sprite(float p_posX, float p_posY,
			   float p_rotX, float p_rotY,
			   float p_scaleX, float p_scaleY,
			   TextureIds::Id p_textureId)
{
	m_position = DirectX::XMFLOAT3(p_posX, p_posY, 0.0f);
	m_rotation = DirectX::XMFLOAT3(p_rotX, p_rotY, 0.0f);
	m_scale = DirectX::XMFLOAT3(p_scaleX, p_scaleY, 1.0f);

	m_textureId = p_textureId;
}
Sprite::~Sprite()
{
}

void Sprite::setPosition(float p_posX, float p_posY)
{
	m_position.x = p_posX;
	m_position.y = p_posY;
}
void Sprite::setRotation(float p_rotX, float p_rotY)
{
	m_rotation.x = p_rotX;
	m_rotation.y = p_rotY;
}
void Sprite::setScale(float p_scaleX, float p_scaleY)
{
	m_scale.x = p_scaleX;
	m_scale.y = p_scaleY;
}

TextureIds::Id Sprite::getTextureId() const
{
	return m_textureId;
}

DirectX::XMFLOAT3 Sprite::getPosition() const
{
	return m_position;
}
DirectX::XMFLOAT3 Sprite::getRotation() const
{
	return m_rotation;
}
DirectX::XMFLOAT3 Sprite::getScale() const
{
	return m_scale;
}

DirectX::XMFLOAT4X4 Sprite::getWorldMatrix()
{
	DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(m_rotation.x);
	DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(m_rotation.y);
	DirectX::XMMATRIX rotation  = rotationX * rotationY;
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	DirectX::XMMATRIX xmWorld = scaling * rotation * translation;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, xmWorld);

	return world;
}