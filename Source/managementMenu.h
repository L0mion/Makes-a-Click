#ifndef MANAGEMENT_MENU_H
#define MANAGEMENT_MENU_H

#include <d3d11.h>

#include "managementSprite.h"

static const int XBOX_360_GAMEPAD_ANALOG_STICK_MAX = 32767;
static const int XBOX_360_GAMEPAD_ANALOG_STICK_MIN = -32768;

class ManagementMenu
{
public:
	ManagementMenu();
	~ManagementMenu();

	void moveHighlighter(double p_analogStickX, double p_analogStickY);
	void setMenu(ManagementSprite::SpriteCollectionIds spriteCollection);

	ManagementSprite* getManagementSprite();

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT initManagementSprite(ID3D11Device* p_device);

	ManagementSprite* m_managementSprite;
};

#endif // MANAGEMENT_MENU_H