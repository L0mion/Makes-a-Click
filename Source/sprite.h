#ifndef SPRITE_H
#define SPRITE_H

#include <DirectXMath.h>

#include "textureIds.h"

class Sprite
{
public:
	Sprite();
	Sprite( float p_posX, float p_posY,
			float p_rotX, float p_rotY,
			float p_scaleX, float p_scaleY,
			TextureIds::Id p_textureId);
	~Sprite();

	void setPosition(float p_posX, float p_posY);
	void setRotation(float p_rotX, float p_rotY);
	void setScale(float p_scaleX, float p_scaleY);

	TextureIds::Id getTextureId() const;

	DirectX::XMFLOAT3 getPosition() const;
	DirectX::XMFLOAT3 getRotation() const;
	DirectX::XMFLOAT3 getScale() const;
	
	DirectX::XMFLOAT4X4 getWorldMatrix();
private:

	TextureIds::Id m_textureId;

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;

	DirectX::XMFLOAT4X4 m_worldMatrix;
};

#endif //SPRITE_H