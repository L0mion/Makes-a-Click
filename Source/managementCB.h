#ifndef MANAGEMENT_CB_H
#define MANAGEMENT_CB_H

#include <d3d11.h>
#include <DirectXMath.h>

enum CBType
{
	CB_TYPE_FRAME
};



static const unsigned int CB_REGISTER_FRAME = 0;

class ManagementCB
{
public:
	ManagementCB();
	~ManagementCB();

	void vsSetCB(ID3D11DeviceContext* devcon, CBType cbType);
	void psSetCB(ID3D11DeviceContext* devcon, CBType cbType);

	void updateCBFrame(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 finalMatrix);

	HRESULT init(ID3D11Device* device);
private:
	HRESULT initCBFrame(ID3D11Device* device);

	ID3D11Buffer* cbFrame_;
};

#endif //MANAGEMENT_CB_H