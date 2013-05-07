#ifndef MANAGEMENT_SHADER_H
#define MANAGEMENT_SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

struct ShaderSet
{
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader*	m_ps;

	ID3DBlob* m_vsBlob;
	ID3DBlob* m_psBlob;

	ID3D11InputLayout* m_inputLayout;
};

class ManagementShader
{
public:
	ManagementShader();
	~ManagementShader();

	enum ShaderIds
	{
		ShaderIds_VS_DEFAULT,
		ShaderIds_VS_SPRITE,
		ShaderIds_VS_HEIGHTMAP,
		ShaderIds_PS_DEFAULT,
		ShaderIds_PS_SPRITE,
		ShaderIds_PS_HEIGHTMAP
	};
	
	enum InputLayoutIds
	{
		InputLayoutIds_DEFAULT,
		InputLayoutIds_SPRITE,
		InputLayoutIds_HEIGHTMAP
	};

	void setShader(ID3D11DeviceContext* devcon, ShaderIds shaderId);
	void setInputLayout(ID3D11DeviceContext* devcon, InputLayoutIds inputLayoutId);

	HRESULT init(ID3D11Device* device);
private:
	HRESULT initShaders(ID3D11Device* device);
	HRESULT initVertexShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11VertexShader** shader, ID3DBlob** blob);
	HRESULT initPixelShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11PixelShader** shader, ID3DBlob** blob);

	HRESULT initInputLayouts(ID3D11Device* device);
	HRESULT initVSDefaultInputLayout(ID3D11Device* device);
	HRESULT initVSSpriteInputLayout(ID3D11Device* device);
	HRESULT initVSHeightmapInputLayout(ID3D11Device* device);

	std::wstring decideFilePath();

	ID3D11VertexShader* vsDefault_;
	ID3D11VertexShader* vsSprite_;
	ID3D11PixelShader*	psDefault_;
	ID3D11PixelShader*	psSprite_;

	ID3DBlob* vsDefaultBlob_;
	ID3DBlob* vsSpriteBlob_;
	ID3DBlob* psDefaultBlob_;
	ID3DBlob* psSpriteBlob_;

	ID3D11InputLayout* vsDefaultIL_;
	ID3D11InputLayout* vsSpriteIL_;
};

#endif //MANAGEMENT_SHADER_H