#ifndef INPUT_DESC_H
#define INPUT_DESC_H

#include <vector>

struct InputDesc
{
	std::vector<bool> keys_;
	int mouseDeltaX_;
	int mouseDeltaY_;

	InputDesc()
	{
		mouseDeltaX_ = 0;
		mouseDeltaY_ = 0;
	}
	InputDesc(std::vector<bool> keys, int mouseDeltaX, int mouseDeltaY)
	{
		keys_ = keys;
		mouseDeltaX_ = mouseDeltaX;
		mouseDeltaY_ = mouseDeltaY;
	}
};

#endif //INPUT_DESC_H