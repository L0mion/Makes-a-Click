#ifndef MANAGEMENT_CB_H
#define MANAGEMENT_CB_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "cbDesc.h"

class ManagementCB
{
public:
	ManagementCB();
	~ManagementCB();

	void vsSetCB(ID3D11DeviceContext* p_devcon, CBTypes p_cbType);
	void psSetCB(ID3D11DeviceContext* p_devcon, CBTypes p_cbType);

	void updateCBFrame(ID3D11DeviceContext* p_devcon,
		DirectX::XMFLOAT4X4 p_finalMatrix, DirectX::XMFLOAT3 p_cameraPos );
	void updateCBSprite(ID3D11DeviceContext* p_devcon, DirectX::XMFLOAT4X4 transformMatrix);
	void updateCBObject(ID3D11DeviceContext* p_devcon, DirectX::XMFLOAT4X4 p_worldMat);

	HRESULT init(ID3D11Device* device);
private:
	HRESULT initConstantBuffer(ID3D11Device* device, ID3D11Buffer** constantBuffer, unsigned int size);

	ID3D11Buffer* cbFrame_;
	ID3D11Buffer* cbSprite_;
	ID3D11Buffer* m_cbObject;
};

#endif //MANAGEMENT_CB_H