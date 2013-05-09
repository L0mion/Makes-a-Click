#include "managementMenu.h"

#include "sprite.h"
#include "utility.h"

ManagementMenu::ManagementMenu()
{
	m_managementSprite = NULL;
}
ManagementMenu::~ManagementMenu()
{
	SAFE_DELETE(m_managementSprite);
}

void ManagementMenu::moveHighlighter(double p_analogStickX, double p_analogStickY)
{
	Sprite* circle		= m_managementSprite->getSprite(ManagementSprite::SpriteIds_CIRCLE_BACKGROUND);
	Sprite* highlighter = m_managementSprite->getSprite(ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);

	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	float height = circle->getScale().y - highlighter->getScale().y;
	float width = height / aspectRatio;

	if(insideSector0(p_analogStickX, p_analogStickY))	
		setHighlighterSector0(highlighter, width, height);

	else if(insideSector1(p_analogStickX, p_analogStickY))
		setHighlighterSector1(highlighter, width, height);
	
	else if(insideSector2(p_analogStickX, p_analogStickY))
		setHighlighterSector2(highlighter, width, height);
	
	else if(insideSector3(p_analogStickX, p_analogStickY))
		setHighlighterSector3(highlighter, width, height);	

	else if(insideSector4(p_analogStickX, p_analogStickY))
		setHighlighterSector4(highlighter, width, height);

	else if(insideSector5(p_analogStickX, p_analogStickY))
		setHighlighterSector5(highlighter, width, height);

	else if(insideSector6(p_analogStickX, p_analogStickY))
		setHighlighterSector6(highlighter, width, height);

	else if(insideSector7(p_analogStickX, p_analogStickY))
		setHighlighterSector7(highlighter, width, height);
		
	else
		highlighter->setPosition(0.0f, 0.0f);
}

void ManagementMenu::setMenuSprites(ManagementSprite::SpriteCollectionIds spriteCollection)
{
	m_managementSprite->setSpriteCollection(spriteCollection);
}

ManagementSprite* ManagementMenu::getManagementSprite()
{
	return m_managementSprite;
}

HRESULT ManagementMenu::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	hr = initManagementSprite(p_device);

	return hr;
}
HRESULT ManagementMenu::initManagementSprite(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	m_managementSprite = new ManagementSprite();
	m_managementSprite->init(p_device);
	return hr;
}

bool ManagementMenu::insideSector0(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if((p_analogX < 0.5 && p_analogX > -0.5) && p_analogY > 0.5f)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector1(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > 0.5f && p_analogY > 0.5f)
		isInside = true;
	
	return isInside;
}
bool ManagementMenu::insideSector2(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > 0.5f && (p_analogY > -0.5f && p_analogY < 0.5f))
		isInside = true;
	
	return isInside;
}
bool ManagementMenu::insideSector3(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > 0.5f && p_analogY < -0.5f)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector4(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if((p_analogX < 0.5f && p_analogX > -0.5f) && p_analogY < -0.5f)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector5(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -0.5f && p_analogY < -0.5f)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector6(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -0.5f &&( p_analogY < 0.5f && p_analogY > -0.5f))
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector7(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -0.5f && p_analogY > 0.5f)
		isInside = true;

	return isInside;
}

void ManagementMenu::setHighlighterSector0(Sprite* highlighter, float width, float height)
{
	float posX = 0.0f;
	float posY = height;
	highlighter->setPosition(posX, posY);
}
void ManagementMenu::setHighlighterSector1(Sprite* highlighter, float width, float height)
{
	float posX = width * sin(DirectX::XM_PIDIV4);
	float posY = height * cos(DirectX::XM_PIDIV4);
	highlighter->setPosition(posX, posY);
}
void ManagementMenu::setHighlighterSector2(Sprite* highlighter, float width, float height)
{
	float posX = width;
	float posY = 0.0f;
	highlighter->setPosition(posX, posY);
}
void ManagementMenu::setHighlighterSector3(Sprite* highlighter, float width, float height)
{
	float posX = width * sin(DirectX::XM_PIDIV4);
	float posY = -height * cos(DirectX::XM_PIDIV4);
	highlighter->setPosition(posX, posY);
}
void ManagementMenu::setHighlighterSector4(Sprite* highlighter, float width, float height)
{
	float posX = 0.0f;
	float posY = -height;
	highlighter->setPosition(posX, posY);
}
void ManagementMenu::setHighlighterSector5(Sprite* highlighter, float width, float height)
{
	float posX = -width * sin(DirectX::XM_PIDIV4);
	float posY = -height * cos(DirectX::XM_PIDIV4);
	highlighter->setPosition(posX, posY);
}
void ManagementMenu::setHighlighterSector6(Sprite* highlighter, float width, float height)
{
	float posX = -width;
	float posY = 0.0f;
	highlighter->setPosition(posX, posY);
}
void ManagementMenu::setHighlighterSector7(Sprite* highlighter, float width, float height)
{
	float posX = -width * sin(DirectX::XM_PIDIV4);
	float posY = height * cos(DirectX::XM_PIDIV4);
	highlighter->setPosition(posX, posY);
}