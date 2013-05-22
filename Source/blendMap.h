#ifndef BLEND_MAP_H
#define BLEND_MAP_H

#include <vector>
#include <d3d11.h>

#include "texel.h"

class BlendMap
{
public:
	BlendMap();
	~BlendMap();

	void setTexel(ID3D11DeviceContext* p_devcon, Texel p_texel, int p_x, int p_y);
	void setAllTexels(ID3D11DeviceContext* p_devcon, Texel p_texel);

	void psSetBlendMap(ID3D11DeviceContext* p_devcon, unsigned int startSlott);

	ID3D11Texture2D* getTexBlendMap() const;
	ID3D11ShaderResourceView* getSrvBlendMap() const;

	HRESULT init(ID3D11Device* p_device, int p_width, int p_height);
private:
	HRESULT initTexBlendMap(ID3D11Device* p_device, int p_width, int p_height);
	HRESULT initSrvBlendMap(ID3D11Device* p_device);

	int m_width;
	int m_height;
	std::vector<Texel> m_texels;

	ID3D11Texture2D*			m_texBlendMap;
	ID3D11ShaderResourceView*	m_srvBlendMap;
};

#endif //BLEND_MAP_H