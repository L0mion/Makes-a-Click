#ifndef MANAGEMENT_SS_H
#define MANAGEMENT_SS_H

#include <d3d11.h>
#include <vector>

using namespace std;

class ManagementSS
{
public:
	ManagementSS();
	~ManagementSS();

	enum SSTypes
	{
		SSTypes_DEFAULT,
		SSTypes_WRAP,
		SSTypes_CNT
	};

	void setSS(ID3D11DeviceContext* devcon, SSTypes ssType, unsigned int sahderRegister);

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT initSSDefault(ID3D11Device* p_device);
	HRESULT initSSWrap( ID3D11Device* p_device );

	//ID3D11SamplerState* m_ssDefault;
	vector<ID3D11SamplerState*> m_samplerStates;
};

#endif //MANAGEMENT_SS_H