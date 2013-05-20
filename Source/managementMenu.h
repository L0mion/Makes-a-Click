#ifndef MANAGEMENT_MENU_H
#define MANAGEMENT_MENU_H

#include <vector>
#include <d3d11.h>

#include "managementSprite.h"

static const int XBOX_360_GAMEPAD_ANALOG_STICK_MAX = 32767;
static const int XBOX_360_GAMEPAD_ANALOG_STICK_MIN = -32768;

class ManagementWrite;
struct Text;

class ManagementMenu
{
public:
	ManagementMenu();
	~ManagementMenu();

	enum ToolIds
	{
		ToolIds_NONE,
		ToolIds_SAND,
		ToolIds_ROAD,
		ToolIds_OBJECT,

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
		ToolPropertyIds_PROPERTY_7,
	};

	void useToolsMenu(double p_analogStickX, double p_analogStickY);
	void useToolPropertiesMenu(double p_analogStickX, double p_analogStickY);
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

	void moveHighlighter(double p_analogStickX, double p_analogStickY);
	void setHighlighterPos(ManagementSprite::SectorIds sectorId);

	bool insideSector0(double p_analogX, double p_analogY);
	bool insideSector1(double p_analogX, double p_analogY);
	bool insideSector2(double p_analogX, double p_analogY);
	bool insideSector3(double p_analogX, double p_analogY);
	bool insideSector4(double p_analogX, double p_analogY);
	bool insideSector5(double p_analogX, double p_analogY);
	bool insideSector6(double p_analogX, double p_analogY);
	bool insideSector7(double p_analogX, double p_analogY);

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

	DirectX::XMFLOAT2 calcTextPosForCenter(std::wstring p_text);

	DirectX::XMFLOAT4	m_textColor;
	std::vector<Text*>	m_textStrings;
	ManagementSprite*	m_managementSprite;
	ToolIds				m_activeTool;
	ToolIds				m_tempSelectedTool;
	ToolPropertyIds		m_tempSelectedProperty;
	ToolPropertyIds		m_activeProperty;
};

#endif // MANAGEMENT_MENU_H