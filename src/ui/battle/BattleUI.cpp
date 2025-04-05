#include "BattleUI.hpp"

#include "PartyUIElement.hpp"
#include "../../scene/BattleScene.hpp"
#include "../../singleton/SceneManager.hpp"
#include "../../struct/ui/general/TextElement.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Determines how many TextElement instances will be created during the "OnUserCreate" function of the battle UI.						//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BATUI_INITIAL_TEXT_ELEMENTS		16ui64

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines for the position of the party UI within a battle and the offsets for its various elements.									//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define PARTY_UI_X						360.0f
#define PARTY_UI_Y						280.0f
#define PARTY_UI_HP_BAR_X				168.0f
#define PARTY_UI_HP_BAR_Y				10.0f
#define PARTY_UI_MP_BAR_X				216.0f
#define PARTY_UI_MP_BAR_Y				10.0f
#define PARTY_UI_SPACING_Y				16.0f

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines for the position of a given enemy's UI within a battle and the offsets for its various elements.							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define ENEMY_UI_HP_BAR_X			   -4.0f
#define ENEMY_UI_HP_BAR_Y				32.0f
#define ENEMY_UI_MP_BAR_X			   -4.0f
#define ENEMY_UI_MP_BAR_Y				38.0f

BattleUI::BattleUI() :
	uiElements(),
	textElements(),
	updateTimer(0.0f),
	activeTextElementCount(0ui32)
{ // Reserve a bit of memory to account for potential of simultaneous damage values being on screen at once.
	textElements.reserve(BATUI_INITIAL_TEXT_ELEMENTS);
}

void BattleUI::OnUserCreate() {
	// Create the 3 UI elements that are exclusively used by player party members.
	for (size_t j = 0ui64; j < BATTLE_MAX_PARTY_SIZE; j++)
		uiElements[j] = new PartyUIElement();

	// Create the 8 UI elements for each possible enemy that can exist in a battle.
	for (size_t i = BATTLE_MAX_PARTY_SIZE; i < BATTLE_TOTAL_COMBATANTS; i++)
		uiElements[i] = new BattleUIElement();

	// Create the required initial text elements to help mitigate any memory allocations whenever additional text 
	// elements need to be displayed during battle.
	for (size_t k = 0ui64; k < BATUI_INITIAL_TEXT_ELEMENTS; k++)
		textElements.push_back(new TextElement());
}

void BattleUI::OnUserDestroy() {
	size_t _length = textElements.size();
	for (size_t i = 0ui64; i < _length; i++)
		delete textElements[i], textElements[i] = nullptr;
	textElements.clear();
	textElements.shrink_to_fit();

	for (size_t i = 0ui64; i < BATTLE_TOTAL_COMBATANTS; i++)
		delete uiElements[i], uiElements[i] = nullptr;
}

void BattleUI::OnUserUpdate() {
	updateTimer -= EngineCore::deltaTime;
	if (updateTimer > 0.0f)
		return;
	updateTimer = BATUI_UPDATE_INTERVAL;

	for (BattleUIElement* _element : uiElements)
		_element->OnUserUpdate();
}

void BattleUI::OnUserRender(EngineCore* _engine) {
	float_t _deltaTime = EngineCore::deltaTime;
	size_t _length = textElements.size();
	for (uint64_t i = 0ui64; i < _length; i++) {
		if (!textElements[i]->OnUserRender(_engine, _deltaTime))
			activeTextElementCount--;
	}

	for (BattleUIElement* _element : uiElements)
		_element->OnUserRender(_engine);
}

void BattleUI::ActivateElement(Combatant* _combatant, size_t _index) {
	if (_index >= BATTLE_TOTAL_COMBATANTS || (uiElements[_index]->flags & FLAG_BATUI_ELEMENT_IN_USE))
		return;

	if (_index < BATTLE_MAX_PARTY_SIZE) {
		BattleScene* _scene = (BattleScene*)GET_SINGLETON(SceneManager)->curScene;
		PartyUIElement* _pElement = (PartyUIElement*)uiElements[_index];
		_pElement->ActivateElement(
			PARTY_UI_X,
			PARTY_UI_Y + float_t(PARTY_UI_SPACING_Y * uint32_t(_scene->GetTotalPlayerCombatants())),
			PARTY_UI_HP_BAR_X,
			PARTY_UI_HP_BAR_Y,
			PARTY_UI_MP_BAR_X,
			PARTY_UI_MP_BAR_Y,
			_combatant, 
			FLAG_BATUI_ELEMENT_VISIBLE | FLAG_BATUI_ELEMENT_IN_USE | FLAG_BATUI_ELEMENT_HP_SHOWN | FLAG_BATUI_ELEMENT_MP_SHOWN,
			_scene
		);
		return;
	}

	auto& _coords = BattleScene::positions[_index];
	uiElements[_index]->ActivateElement(
		_coords.first,
		_coords.second,
		ENEMY_UI_HP_BAR_X,
		ENEMY_UI_HP_BAR_Y,
		ENEMY_UI_MP_BAR_X,
		ENEMY_UI_MP_BAR_Y,
		_combatant,
		FLAG_BATUI_ELEMENT_IN_USE | FLAG_BATUI_ELEMENT_USE_TIMER | FLAG_BATUI_ELEMENT_HP_SHOWN
	);
}

void BattleUI::CreateText(const std::string& _string, float_t _moveSpeed, float_t _lifetime, olc::Pixel _color, float_t _xOffset, float_t _yOffset, float_t _scale) {
	size_t _length		= _string.size();
	float_t _strOffsetX	= 8.0f * _length / 2.0f;
	if (activeTextElementCount < textElements.size()) {
		for (TextElement* _text : textElements) {
			if (_text->lifetime == 0.0f) {
				_text->EnableElement(_xOffset - _strOffsetX, _yOffset, _string, _moveSpeed, _lifetime, _color, _scale);
				activeTextElementCount++;
				break;
			}
		}
		return;
	}

	TextElement* _text	= new TextElement(_xOffset - _strOffsetX, _yOffset, _string, _moveSpeed, _lifetime, _color, _scale);
	textElements.push_back(_text);
	activeTextElementCount++;
}

void BattleUI::CreateSkillNameText(const std::string& _name) {
	CreateText(_name, 0.0f, 60.0f, COLOR_WHITE, VIEWPORT_WIDTH_F / 2.0f, (VIEWPORT_HEIGHT_F / 2.0f) - 150.0f);
}