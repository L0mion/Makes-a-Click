#include "managementWrite.h"

#include "utility.h"

ManagementWrite::ManagementWrite()
{
	m_spriteBatch = NULL;
	m_spriteFont = NULL;
}
ManagementWrite::~ManagementWrite()
{
	SAFE_DELETE(m_spriteBatch);
	SAFE_DELETE(m_spriteFont);
}

void ManagementWrite::renderText()
{
	m_spriteBatch->Begin();

	DirectX::FXMVECTOR xmColor = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	m_spriteFont->DrawString(m_spriteBatch, L"Hello World!", DirectX::XMFLOAT2(50, 50), xmColor);

	m_spriteBatch->End();
}

void ManagementWrite::init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon)
{
	m_spriteBatch = new DirectX::SpriteBatch( p_devcon );
	m_spriteFont = new DirectX::SpriteFont( p_device, L"../../Resources/Fonts/Calibri.spritefont" );
}