#ifndef BLEND_MAP_H
#define BLEND_MAP_H

#include <vector>
#include <d3d11.h>

#include "managementMenu.h"
#include "texel.h"

class PivotPoint;
class HeightMap;

class BlendMap
{
public:
	BlendMap();
	~BlendMap();

	void update(ID3D11DeviceContext* p_devcon,
		PivotPoint* p_pivot,
		HeightMap* p_heightMap,
		ManagementMenu::ToolPropertyIds toolProperty,
		float p_dt);

	void setTexel(ID3D11DeviceContext* p_devcon, Texel p_texel, int p_x, int p_y);
	void setAllTexels(ID3D11DeviceContext* p_devcon, Texel p_texel);

	void psSetBlendMap(ID3D11DeviceContext* p_devcon, unsigned int startSlott);

	ID3D11Texture2D* getTexBlendMap() const;
	ID3D11ShaderResourceView* getSrvBlendMap() const;

	HRESULT init(ID3D11Device* p_device, int p_width, int p_height);
private:
	HRESULT initTexBlendMap(ID3D11Device* p_device, int p_width, int p_height);
	HRESULT initSrvBlendMap(ID3D11Device* p_device);

	HRESULT updateTexture(ID3D11DeviceContext* p_devcon);

	void modifyTexelChannel(float* p_channel, float p_value);

	bool insideCircle(float p_radius, int p_x, int p_z, int p_col, int p_row);

	int m_width;
	int m_height;
	int m_numTexels;
	std::vector<Texel> m_texels;

	ID3D11Texture2D*			m_texBlendMap;
	ID3D11ShaderResourceView*	m_srvBlendMap;
};

#endif //BLEND_MAP_H