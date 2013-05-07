#ifndef MANAGEMENT_TEX_H
#define MANAGEMENT_TEX_H

#include <vector>
#include <string>

#include <d3d11.h>

#include "textureIds.h"

class ManagementTex
{
public:
	ManagementTex();
	~ManagementTex();

	void psSetTexture(ID3D11DeviceContext* p_devcon, TextureIds::Id p_textureId, unsigned int p_shaderRegister);

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT loadTexture(ID3D11Device* p_device, std::wstring p_filename, TextureIds::Id textureId);


	std::vector<ID3D11ShaderResourceView*> m_srvTextures;

};

#endif //MANAGEMENT_TEX_H