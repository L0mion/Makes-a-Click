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
	void setMenuSprites(ManagementSprite::SpriteCollectionIds spriteCollection);

	ManagementSprite* getManagementSprite();

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT initManagementSprite(ID3D11Device* p_device);

	bool insideSector0(double p_analogX, double p_analogY);
	bool insideSector1(double p_analogX, double p_analogY);
	bool insideSector2(double p_analogX, double p_analogY);
	bool insideSector3(double p_analogX, double p_analogY);
	bool insideSector4(double p_analogX, double p_analogY);
	bool insideSector5(double p_analogX, double p_analogY);
	bool insideSector6(double p_analogX, double p_analogY);
	bool insideSector7(double p_analogX, double p_analogY);

	void setHighlighterSector0(Sprite* highlighter, float width, float height);
	void setHighlighterSector1(Sprite* highlighter, float width, float height);
	void setHighlighterSector2(Sprite* highlighter, float width, float height);
	void setHighlighterSector3(Sprite* highlighter, float width, float height);
	void setHighlighterSector4(Sprite* highlighter, float width, float height);
	void setHighlighterSector5(Sprite* highlighter, float width, float height);
	void setHighlighterSector6(Sprite* highlighter, float width, float height);
	void setHighlighterSector7(Sprite* highlighter, float width, float height);

	ManagementSprite* m_managementSprite;
};

#endif // MANAGEMENT_MENU_H