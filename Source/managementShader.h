#ifndef MANAGEMENT_SHADER_H
#define MANAGEMENT_SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

enum ShaderID
{
	SHADER_ID_CS_PRIMARY_RAY_STAGE
};

class ManagementShader
{
public:
	ManagementShader();
	~ManagementShader();

	void setShader(ShaderID shaderID, ID3D11DeviceContext* devcon);

	HRESULT init(ID3D11Device* device);
private:
	HRESULT initComputeShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11ComputeShader** shader);

	ID3D11ComputeShader* csPrimaryRayStage_;

	bool debugShaders_;
};

#endif //MANAGEMENT_SHADER_H