#ifndef MANAGEMENT_WRITE_H
#define MANAGEMENT_WRITE_H

#include <d3d11.h>

#include <SpriteBatch.h>
#include <SpriteFont.h>

struct Text;

class ManagementWrite
{
public:
	ManagementWrite();
	~ManagementWrite();

	void renderText(Text* p_text);

	void init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);
private:

	DirectX::SpriteBatch* m_spriteBatch;
	DirectX::SpriteFont* m_spriteFont;
	
};

#endif //MANAGEMENT_WRITE_H