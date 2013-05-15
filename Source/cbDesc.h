#ifndef CB_DESC_H
#define CB_DESC_H

#include <DirectXMath.h>

enum CBTypes
{
	CBTypes_FRAME,
	CBTypes_SPRITE,
	CBTypes_OBJECT
};

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

struct CBObject
{
	DirectX::XMFLOAT4X4 m_world;
};
static const unsigned int CB_OBJECT_SIZE = 64;
static const unsigned int CB_OBJECT_REGISTER = 2;

#endif //CB_DESC_H