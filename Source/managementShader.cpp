#include "managementShader.h"
#include "utility.h"

ManagementShader::ManagementShader()
{
	vsDefault_ = NULL;
	psDefault_ = NULL;

	vsDefaultBlob_ = NULL;
	psDefaultBlob_ = NULL;

	vsDefaultIL_ = NULL;
}
ManagementShader::~ManagementShader()
{
	SAFE_RELEASE(vsDefault_);
	SAFE_RELEASE(psDefault_);

	SAFE_RELEASE(vsDefaultBlob_);
	SAFE_RELEASE(psDefaultBlob_);

	SAFE_RELEASE(vsDefaultIL_);
}

void ManagementShader::setShader(ID3D11DeviceContext* devcon, ShaderId shaderId)
{
	switch(shaderId)
	{
	case SHADER_ID_VS_DEFAULT:
		devcon->VSSetShader(vsDefault_, NULL, 0);
		break;
	case SHADER_ID_PS_DEFAULT:
		devcon->PSSetShader(psDefault_, NULL, 0);
		break;
	}
}
void ManagementShader::setInputLayout(ID3D11DeviceContext* devcon, InputLayoutId inputLayoutId)
{
	switch(inputLayoutId)
	{
	case INPUT_LAYOUT_ID_VS_DEFAULT:
		devcon->IASetInputLayout(vsDefaultIL_);
		break;
	default:
		devcon->IASetInputLayout(NULL);
		break;
	}
}

HRESULT ManagementShader::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	
	hr = initShaders(device);
	if(SUCCEEDED(hr))
		hr = initInputLayouts(device);

	return hr;
}
HRESULT ManagementShader::initShaders(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	std::wstring filePath = decideFilePath();

	hr = initVertexShader(device, filePath, L"vsDefault.cso", &vsDefault_, &vsDefaultBlob_);
	if(SUCCEEDED(hr))
		hr = initPixelShader(device, filePath, L"psDefault.cso", &psDefault_, &psDefaultBlob_);
	
	return hr;
}
HRESULT ManagementShader::initVertexShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11VertexShader** shader, ID3DBlob** blob)
{
	HRESULT hr = S_OK;

	std::wstring completePath = filePath + fileName;

	hr = D3DReadFileToBlob(completePath.c_str(), blob);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetShader::initVertexShader() | D3DReadFileToBlob() | Failed", completePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	else
	{
		hr = device->CreateVertexShader((*blob)->GetBufferPointer(), (*blob)->GetBufferSize(), NULL, shader);
		if(FAILED(hr))
			MessageBox(NULL, L"ManagementShader::initVertexShader() | device->CreateVertexShader() | Failed", completePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}

	return hr;
}
HRESULT ManagementShader::initPixelShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11PixelShader** shader, ID3DBlob** blob)
{
	HRESULT hr = S_OK;

	std::wstring completePath = filePath + fileName;

	hr = D3DReadFileToBlob(completePath.c_str(), blob);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetShader::initPixelShader() | D3DReadFileToBlob() | Failed", completePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	else
	{
		hr = device->CreatePixelShader((*blob)->GetBufferPointer(), (*blob)->GetBufferSize(), NULL, shader);
		if(FAILED(hr))
			MessageBox(NULL, L"ManagementShader::initPixelShader() | device->CreatePixelShader() | Failed", completePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}

	return hr;
}

HRESULT ManagementShader::initInputLayouts(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initVSDefaultInputLayout(device);

	return hr; 
}
HRESULT ManagementShader::initVSDefaultInputLayout(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = device->CreateInputLayout(ied, 2, vsDefaultBlob_->GetBufferPointer(), vsDefaultBlob_->GetBufferSize(), &vsDefaultIL_);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagementShader::initVSDefaultInputLayout() | device->CreateInputLayout() | Failed", L"vsDefaultIL", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}

std::wstring ManagementShader::decideFilePath()
{
#if defined( DEBUG ) || defined( _DEBUG )
	bool debugShaders = true;
#else
	bool debugShaders = false;
#endif

	std::wstring filePath;
	if(debugShaders)
		filePath = L"../Debug/";
	else
		filePath = L"../Release/";

	return filePath;
}