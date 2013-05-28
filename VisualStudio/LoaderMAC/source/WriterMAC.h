#pragma once

struct Mac;

#include <Util.h>

class WriterMAC {
public:
	WriterMAC( Mac& io_mac );
	~WriterMAC();

	bool init();
protected:
private:
	void uglyHeightmapToRaw( 
		std::vector<float>& p_heightmap, 
		float p_scale, 
		float p_offset, 
		std::vector<char>& io_raw );
	void uglyBlendmapToRaw(
		std::vector<Util::Texel>& p_blendmap,
		unsigned int p_row,
		unsigned int p_col,
		std::vector<char>& io_raw);

	Mac* m_mac;
};