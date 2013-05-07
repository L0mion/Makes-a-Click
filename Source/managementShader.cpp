#include "managementShader.h"
#include "utility.h"

ShaderSet::ShaderSet()
{
	setNull();
}

void ShaderSet::setNull()
{
	m_vs			= NULL;
	m_ps			= NULL;
	m_vsBlob		= NULL;
	m_psBlob		= NULL;
	m_inputLayout	= NULL;
}

void ShaderSet::safeRelease()
{
	SAFE_RELEASE( m_vs );
	SAFE_RELEASE( m_ps );
	SAFE_RELEASE( m_vsBlob );
	SAFE_RELEASE( m_psBlob );
	SAFE_RELEASE( m_inputLayout );
}

void ShaderSet::set( ID3D11DeviceContext* p_devcon )
{
	p_devcon->VSSetShader( m_vs, NULL, 0 );
	p_devcon->PSSetShader( m_ps, NULL, 0 );
	p_devcon->IASetInputLayout( m_inputLayout );
}

ManagementShader::ManagementShader()
{
	for( int i=0; i < ShaderIds_CNT; i++ ) {
		m_shaders[i].setNull();
	}
}
ManagementShader::~ManagementShader()
{
	for( int i=0; i < ShaderIds_CNT; i++ ) {
		m_shaders[i].safeRelease();
	}
}

void ManagementShader::setShader(ID3D11DeviceContext* p_devcon,
	ShaderIds p_shaderId)
{
	m_shaders[p_shaderId].set( p_devcon );
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

	hr = initVertexShader( device, filePath, L"vsDefault.cso",
		&m_shaders[ShaderIds_DEFAULT].m_vs,
		&m_shaders[ShaderIds_DEFAULT].m_vsBlob );
	if(SUCCEEDED(hr))
		hr = initVertexShader( device, filePath, L"vsSprite.cso",
			&m_shaders[ShaderIds_SPRITE].m_vs,
			&m_shaders[ShaderIds_SPRITE].m_vsBlob );
	if(SUCCEEDED(hr))
		hr = initVertexShader( device, filePath, L"vsHeightMap.cso",
		&m_shaders[ShaderIds_HEIGHTMAP].m_vs,
		&m_shaders[ShaderIds_HEIGHTMAP].m_vsBlob );
	if(SUCCEEDED(hr))
		hr = initPixelShader( device, filePath, L"psDefault.cso",
			&m_shaders[ShaderIds_DEFAULT].m_ps,
			&m_shaders[ShaderIds_DEFAULT].m_psBlob );
	if(SUCCEEDED(hr))
		hr = initPixelShader( device, filePath, L"psSprite.cso",
			&m_shaders[ShaderIds_SPRITE].m_ps,
			&m_shaders[ShaderIds_SPRITE].m_psBlob );
	if(SUCCEEDED(hr))
		hr = initPixelShader( device, filePath, L"psHeightMap.cso",
		&m_shaders[ShaderIds_HEIGHTMAP].m_ps,
		&m_shaders[ShaderIds_HEIGHTMAP].m_psBlob );

	return hr;
}
HRESULT ManagementShader::initVertexShader( ID3D11Device* device,
	std::wstring filePath, std::wstring fileName,
	ID3D11VertexShader** shader, ID3DBlob** blob )
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

	hr = initVSDefaultInputLayout( device );
	if( SUCCEEDED(hr) ) {
		hr = initVSSpriteInputLayout( device );
	} if( SUCCEEDED(hr) ) {
		//hr = initVSHeightmapInputLayout( device );
	}
	return hr; 
}

HRESULT ManagementShader::initVSDefaultInputLayout(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	int elementCnt = sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	hr = device->CreateInputLayout( ied, elementCnt,
		m_shaders[ShaderIds_DEFAULT].m_vsBlob->GetBufferPointer(),
		m_shaders[ShaderIds_DEFAULT].m_vsBlob->GetBufferSize(),
		&m_shaders[ShaderIds_DEFAULT].m_inputLayout );
	if(FAILED(hr))
		MessageBox(NULL, L"ManagementShader::initVSDefaultInputLayout() | device->CreateInputLayout() | Failed", L"vsDefaultIL", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}

HRESULT ManagementShader::initVSSpriteInputLayout(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	int elementCnt = sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	hr = device->CreateInputLayout( ied, elementCnt,
		m_shaders[ShaderIds_SPRITE].m_vsBlob->GetBufferPointer(),
		m_shaders[ShaderIds_SPRITE].m_vsBlob->GetBufferSize(),
		&m_shaders[ShaderIds_SPRITE].m_inputLayout );

	if(FAILED(hr))
		MessageBox(NULL, L"ManagementShader::initVSSpriteInputLayout() | device->CreateInputLayout() | Failed", L"vsSpritetIL", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}

HRESULT ManagementShader::initVSHeightmapInputLayout( ID3D11Device* device )
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	int elementCnt = sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	HRESULT hr = S_OK;
	hr = device->CreateInputLayout( ied, elementCnt,
		m_shaders[ShaderIds_HEIGHTMAP].m_vsBlob->GetBufferPointer(),
		m_shaders[ShaderIds_HEIGHTMAP].m_vsBlob->GetBufferSize(),
		&m_shaders[ShaderIds_HEIGHTMAP].m_inputLayout );

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
