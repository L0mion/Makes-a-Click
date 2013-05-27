#ifndef MANAGEMENT_MENU_H
#define MANAGEMENT_MENU_H

#include <string>
#include <vector>
#include <d3d11.h>

#include "managementSprite.h"

class ManagementWrite;
class XInputFetcher;
struct Text;

class ManagementMenu
{
public:
	ManagementMenu(XInputFetcher* p_xinput);
	~ManagementMenu();

	enum ToolIds
	{
		ToolIds_NONE,
		ToolIds_SAND,
		ToolIds_TEXTURE_BRUSH,
		ToolIds_OBJECT
	};
	enum ToolPropertyIds
	{
		ToolPropertyIds_NONE,
		ToolPropertyIds_PROPERTY_0,
		ToolPropertyIds_PROPERTY_1,
		ToolPropertyIds_PROPERTY_2,
		ToolPropertyIds_PROPERTY_3,
		ToolPropertyIds_PROPERTY_4,
		ToolPropertyIds_PROPERTY_5,
		ToolPropertyIds_PROPERTY_6,
		ToolPropertyIds_PROPERTY_7
	};
	enum TextStates
	{
		TextStates_LOWER_CASE,
		TextStates_UPPER_CASE,
		TextStates_NUMBERS
	};

	void useTextMenu();
	void useToolsMenu();
	void useToolPropertiesMenu();
	void useNoMenu();

	void setSelectedTool();
	void setSelectedProperty();
	ToolIds getActiveTool();
	ToolPropertyIds getActiveProperty();

	ManagementSprite* getManagementSprite();
	std::vector<Text*> getTextStrings();

	void clearTextStrings();

	HRESULT init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);
private:
	HRESULT initManagementSprite(ID3D11Device* p_device);
	void initManagementWrite(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);

	void moveSpriteToSector(double p_analogStickX, double p_analogStickY, ManagementSprite::SpriteIds p_spriteId);
	void setSpriteSector(ManagementSprite::SectorIds sectorId, ManagementSprite::SpriteIds p_spriteId);

	void setToolIcon();
	void setToolPropertyIcon();

	bool insideSector0(double p_analogX, double p_analogY);
	bool insideSector1(double p_analogX, double p_analogY);
	bool insideSector2(double p_analogX, double p_analogY);
	bool insideSector3(double p_analogX, double p_analogY);
	bool insideSector4(double p_analogX, double p_analogY);
	bool insideSector5(double p_analogX, double p_analogY);
	bool insideSector6(double p_analogX, double p_analogY);
	bool insideSector7(double p_analogX, double p_analogY);
	bool insideSectorNone(double p_analogX, double p_analogY);

	void textMenuSwitchHighlighter(double p_analogX, double p_analogY);
	void textMenuSwitchCase();
	void textMenuMoveCursor();
	void textMenuCalcCursorPos();
	void textMenuInputText(double p_analogX, double p_analogY);
	void textMenuSector0();
	void textMenuSector1();
	void textMenuSector2();
	void textMenuSector3();
	void textMenuSector4();
	void textMenuSector5();
	void textMenuSector6();
	void textMenuSector7();
	void textMenuSectorNone();

	void toolsMenuSector0();
	void toolsMenuSector1();
	void toolsMenuSector2();
	void toolsMenuSector3();
	void toolsMenuSector4();
	void toolsMenuSector5();
	void toolsMenuSector6();
	void toolsMenuSector7();
	void toolsMenuSectorNone();

	void useSandPropertiesMenu(double p_analogStickX, double p_analogStickY);
	void sandPropertiesSector0();
	void sandPropertiesSector1();
	void sandPropertiesSector2();
	void sandPropertiesSector3();
	void sandPropertiesSector4();
	void sandPropertiesSector5();
	void sandPropertiesSector6();
	void sandPropertiesSector7();
	void sandPropertiesSectorNone();

	void useObjectPropertiesMenu(double p_analogStickX, double p_analogStickY);
	void objectPropertiesSector0();
	void objectPropertiesSector1();
	void objectPropertiesSector2();
	void objectPropertiesSector3();
	void objectPropertiesSector4();
	void objectPropertiesSector5();
	void objectPropertiesSector6();
	void objectPropertiesSector7();
	void objectPropertiesSectorNone();

	void useTextureBrushPropertiesMenu(double p_analogStickX, double p_analogStickY);
	void textureBrushPropertiesSector0();
	void textureBrushPropertiesSector1();
	void textureBrushPropertiesSector2();
	void textureBrushPropertiesSector3();
	void textureBrushPropertiesSector4();
	void textureBrushPropertiesSector5();
	void textureBrushPropertiesSector6();
	void textureBrushPropertiesSector7();
	void textureBrushPropertiesSectorNone();

	DirectX::XMFLOAT2 calcTextPosForCenter(std::wstring p_text);
	DirectX::XMFLOAT2 calcTextMenuOutPos(std::wstring p_text);

	std::wstring		m_textMenuOut;
	DirectX::XMFLOAT4	m_textColor;
	std::vector<Text*>	m_textStrings;
	ManagementSprite*	m_managementSprite;
	ManagementWrite*	m_managementWrite;
	ToolIds				m_activeTool;
	ToolIds				m_tempSelectedTool;
	ToolPropertyIds		m_tempSelectedProperty;
	ToolPropertyIds		m_activeProperty;
	TextStates			m_textState;
	XInputFetcher*		m_xinput;

	int m_cursorIndex;

	double		m_analogActive;
};

#endif // MANAGEMENT_MENU_H