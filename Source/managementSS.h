#ifndef MANAGEMENT_SS_H
#define MANAGEMENT_SS_H

#include <d3d11.h>

class ManagementSS
{
public:
	ManagementSS();
	~ManagementSS();

	enum SSTypes
	{
		SSTypes_DEFAULT
	};

	void setSS(ID3D11DeviceContext* devcon, SSTypes ssType, unsigned int sahderRegister);

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT initSSDefault(ID3D11Device* p_device);

	ID3D11SamplerState* m_ssDefault;
};

#endif //MANAGEMENT_SS_H