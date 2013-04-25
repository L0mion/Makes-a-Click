#ifndef CB_DESC_H
#define CB_DESC_H

#include <DirectXMath.h>

struct CBFrame
{
	DirectX::XMFLOAT4X4 viewMatrix_;
	DirectX::XMFLOAT4X4 projectionMatrix_;
};
static const unsigned int CB_FRAME_SIZE = 128;

#endif //CB_DESC_H