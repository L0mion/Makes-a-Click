#ifndef INPUTCONTAINER_H
#define INPUTCONTAINER_H

#include <vector>

#include "Utility.h"

using namespace std;

struct InputContainer
{
	vector<bool> keys;
	int mouseDeltaX;
	int mouseDeltaY;

	InputContainer(vector<bool> keys, int mouseDeltaX, int mouseDeltaY)
	{
		this->keys			= keys;
		this->mouseDeltaX	= mouseDeltaX;
		this->mouseDeltaY	= mouseDeltaY;
	}
};

#endif