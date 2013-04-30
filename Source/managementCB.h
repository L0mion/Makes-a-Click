#ifndef MANAGEMENT_CB_H
#define MANAGEMENT_CB_H

#include <d3d11.h>
#include <DirectXMath.h>

class ManagementCB
{
public:
	ManagementCB();
	~ManagementCB();

	enum CBTypes
	{
		CBTypes_FRAME,
		CBTypes_SPRITE
	};

	void vsSetCB(ID3D11DeviceContext* devcon, CBTypes cbType);
	void psSetCB(ID3D11DeviceContext* devcon, CBTypes cbType);

	void updateCBFrame(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 finalMatrix);
	void updateCBSprite(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 transformMatrix);

	HRESULT init(ID3D11Device* device);
private:
	HRESULT initConstantBuffer(ID3D11Device* device, ID3D11Buffer** constantBuffer, unsigned int size);

	ID3D11Buffer* cbFrame_;
	ID3D11Buffer* cbSprite_;
};

#endif //MANAGEMENT_CB_H