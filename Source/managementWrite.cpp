#include "managementWrite.h"

#include "text.h"
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

void ManagementWrite::renderText(Text* p_text)
{
	m_spriteBatch->Begin();

	DirectX::FXMVECTOR xmColor = DirectX::XMLoadFloat4(&p_text->m_color);
	m_spriteFont->DrawString(m_spriteBatch, p_text->m_text.c_str(), p_text->m_position, xmColor);

	m_spriteBatch->End();
}

void ManagementWrite::init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon)
{
	m_spriteBatch = new DirectX::SpriteBatch( p_devcon );
	m_spriteFont = new DirectX::SpriteFont( p_device, L"../../Resources/Fonts/Calibri.spritefont" );
}