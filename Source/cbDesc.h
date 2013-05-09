#ifndef CB_DESC_H
#define CB_DESC_H

#include <DirectXMath.h>

struct CBFrame
{
	DirectX::XMFLOAT4X4 m_finalMatrix;
	DirectX::XMFLOAT3 m_cameraPos;
	float pad;
};
static const unsigned int CB_FRAME_SIZE = 80;
static const unsigned int CB_FRAME_REGISTER = 0;

struct CBSprite
{
	DirectX::XMFLOAT4X4 spriteTransform_;
};
static const unsigned int CB_SPRITE_SIZE = 64;
static const unsigned int CB_SPRITE_REGISTER = 1;

#endif //CB_DESC_H