#ifndef MANAGEMENT_CB_H
#define MANAGEMENT_CB_H

#include <d3d11.h>
#include <DirectXMath.h>

class ManagementCB
{
public:
	ManagementCB();
	~ManagementCB();

	void updateCBFrame(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	HRESULT init(ID3D11Device* device);
private:
	HRESULT initCBFrame(ID3D11Device* device);

	ID3D11Buffer* cbFrame_;
};

#endif //MANAGEMENT_CB_H