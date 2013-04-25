#include "managementShader.h"
#include "utility.h"

ManagementShader::ManagementShader()
{
	csPrimaryRayStage_ = NULL;

#if defined( DEBUG ) || defined( _DEBUG )
	debugShaders_ = true;
#else
	debugShaders_ = false;
#endif
}
ManagementShader::~ManagementShader()
{
	SAFE_RELEASE(csPrimaryRayStage_);
}

void ManagementShader::setShader(ShaderID shaderID, ID3D11DeviceContext* devcon)
{
	switch(shaderID)
	{
	case SHADER_ID_CS_PRIMARY_RAY_STAGE:
		devcon->CSSetShader(csPrimaryRayStage_, 0, 0);
		break;
	}
}

HRESULT ManagementShader::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	std::wstring filePath;
	if(debugShaders_)
		filePath = L"../Debug/";
	else
		filePath = L"../Release/";

	hr = initComputeShader(device, filePath, L"csPrimaryRayStage.cso", &csPrimaryRayStage_);

	return hr;
}
HRESULT ManagementShader::initComputeShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11ComputeShader** shader)
{
	HRESULT hr = S_OK;

	std::wstring completePath = filePath + fileName;

	ID3DBlob* blob;
	hr = D3DReadFileToBlob(completePath.c_str(), &blob);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetShader::initComputeShader() | D3DReadFileToBlob() | Failed", completePath.c_str(), MB_OK | MB_ICONEXCLAMATION); 
	else
	{
		hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, shader);
		if(FAILED(hr))
			MessageBox(NULL, L"ManagementShader::initComputeShader() | device->CreateComputeShader() | Failed", completePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}

	return hr;
}