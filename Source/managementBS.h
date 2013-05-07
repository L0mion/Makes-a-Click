#ifndef MANAGEMENT_BS_H
#define MANAGEMENT_BS_H

#include <d3d11.h>

class ManagementBS
{
public:
	ManagementBS();
	~ManagementBS();

	enum BSTypes
	{
		BSTypes_DEFAULT,
		BSTypes_TRANSPARENCY
	};

	void setBlendState(ID3D11DeviceContext* p_devcon, BSTypes p_type);

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT initBSDefault(ID3D11Device* p_device);
	HRESULT initBSTransparency(ID3D11Device* p_device);

	ID3D11BlendState* m_bsDefault;
	ID3D11BlendState* m_bsTransparency;
};

#endif //MANAGEMENT_BS_H