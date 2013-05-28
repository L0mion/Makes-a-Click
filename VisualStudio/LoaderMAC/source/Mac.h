#pragma once

#include <Util.h>

#include <string>
#include <vector>

struct Mac {
	Util::Mac macDesc;
	std::vector<float> heightmap;
	std::vector<Util::Texel> blendmap;
};