#ifndef CB_DESC_H
#define CB_DESC_H

#include <DirectXMath.h>

struct CBFrame
{
	DirectX::XMFLOAT4X4 finalMatrix_;
};
static const unsigned int CB_FRAME_SIZE = 64;

#endif //CB_DESC_H