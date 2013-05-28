#pragma once

struct Mac;

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

	Mac* m_mac;
};