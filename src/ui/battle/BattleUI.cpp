#include "BattleUI.hpp"

#include "PartyUIElement.hpp"
#include "../../scene/BattleScene.hpp"
#include "../../struct/ui/general/TextElement.hpp"

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
	totalPartyMembers(0ui8),
	updateTimer(0.0f)
{ // Reserve a bit of memory to account for potential of simultaneous damage values being on screen at once.
	textElements.reserve(10ui64);
}

void BattleUI::OnUserCreate() {
	// Create the 3 UI elements that are exclusively used by player party members.
	for (size_t j = 0ui64; j < BATTLE_MAX_PARTY_SIZE; j++)
		uiElements[j] = new PartyUIElement();

	// Create the 8 UI elements for each possible enemy that can exist in a battle.
	for (size_t i = BATTLE_MAX_PARTY_SIZE; i < BATTLE_TOTAL_COMBATANTS; i++)
		uiElements[i] = new BattleUIElement();
}

void BattleUI::OnUserDestroy() {
	size_t _length = textElements.size();
	for (size_t i = 0ui64; i < _length; i++)
		delete textElements[i].second, textElements[i].second = nullptr;
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
	float_t _deltaTime	= EngineCore::deltaTime;
	size_t _index		= 0ui64;
	for (auto _it = textElements.begin(); _it != textElements.end();) {
		auto& _text		= textElements[_index];
		_text.first    -= _deltaTime;
		if (_text.first <= 0.0f) {
			delete _text.second, _text.second = nullptr;
			_it = textElements.erase(_it);
			continue;
		}

		if (_text.second) { // Render the text and slowly move it up the screen.
			_text.second->OnUserRender(_engine);
			_text.second->yOffset -= 8.0f * _deltaTime;
		}
		_index++;
		_it++;
	}

	for (BattleUIElement* _element : uiElements)
		_element->OnUserRender(_engine);
}

void BattleUI::ActivateElement(Combatant* _combatant, size_t _index) {
	if (_index >= BATTLE_TOTAL_COMBATANTS || (uiElements[_index]->flags & FLAG_BATUI_ELEMENT_IN_USE))
		return;

	if (_index < BATTLE_MAX_PARTY_SIZE) {
		uiElements[_index]->ActivateElement(
			PARTY_UI_X, 
			PARTY_UI_Y + float_t(PARTY_UI_SPACING_Y * totalPartyMembers),
			PARTY_UI_HP_BAR_X, 
			PARTY_UI_HP_BAR_Y, 
			PARTY_UI_MP_BAR_X,
			PARTY_UI_MP_BAR_Y,
			_combatant, 
			FLAG_BATUI_ELEMENT_VISIBLE | FLAG_BATUI_ELEMENT_IN_USE | FLAG_BATUI_ELEMENT_HP_SHOWN | FLAG_BATUI_ELEMENT_MP_SHOWN
		);
		totalPartyMembers++;
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

void BattleUI::CreateText(const std::string& _string, size_t _index, olc::Pixel _color, float_t _xOffset, float_t _yOffset, float_t _scale) {
	size_t _length		= _string.size();
	float_t _strOffsetX	= 8.0f * _length / 2.0f;
	auto& _position		= BattleScene::positions[_index];
	TextElement* _text	= new TextElement(
		_position.first + _xOffset - _strOffsetX,
		_position.second + _yOffset,
		_string,
		_color,
		_scale
	);
	textElements.push_back(std::make_pair(60.0f, _text));
}

void BattleUI::CreateDamageText(uint16_t _value, size_t _index, olc::Pixel _color, float_t _xOffset, float_t _yOffset, float_t _scale) {
	CreateText(std::to_string(_value), _index, _color, _xOffset, _yOffset, _scale);
}