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
	double analogX;
	double analogY;

	if(p_analogStickX > 0)
		analogX = p_analogStickX / XBOX_360_GAMEPAD_ANALOG_STICK_MAX;
	else
		analogX = p_analogStickX / XBOX_360_GAMEPAD_ANALOG_STICK_MIN;

	if(p_analogStickY > 0)
		analogY = p_analogStickY / XBOX_360_GAMEPAD_ANALOG_STICK_MAX;
	else
		analogY = p_analogStickY / XBOX_360_GAMEPAD_ANALOG_STICK_MIN;

	int debug = 0;
	if(p_analogStickX > 1000)
		debug = 1;

	Sprite* sprite = m_managementSprite->getSprite(ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);
	if(analogX < -0.5 && (analogY<0.5 && analogY>-0.5))	
		sprite->setPosition(-0.8f, 0.0f);
	else
		sprite->setPosition(0.0f, 0.0f);
}

void ManagementMenu::setMenu(ManagementSprite::SpriteCollectionIds spriteCollection)
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
