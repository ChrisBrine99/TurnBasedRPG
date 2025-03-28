#include "BattleUI.hpp"

#include "../../struct/user_interface/battle/PartyUIElement.hpp"
#include "../../scene/BattleScene.hpp"
#include "../../singleton/EngineCore.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines for the position of the party UI within a battle and the offsets for its various elements.									//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define PARTY_UI_X						360i32
#define PARTY_UI_Y						280i32
#define PARTY_UI_HP_BAR_X				168i32
#define PARTY_UI_HP_BAR_Y				10i32
#define PARTY_UI_MP_BAR_X				216i32
#define PARTY_UI_MP_BAR_Y				10i32
#define PARTY_UI_SPACING_Y				16i32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines for the position of a given enemy's UI within a battle and the offsets for its various elements.							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define ENEMY_UI_HP_BAR_X				8i32
#define ENEMY_UI_HP_BAR_Y				32i32
#define ENEMY_UI_MP_BAR_X				8i32
#define ENEMY_UI_MP_BAR_Y				38i32

BattleUI::BattleUI() :
	uiElements(),
	totalPartyMembers(0ui8),
	updateTimer(0.0f)
{}

void BattleUI::OnUserCreate() {
	// Create the 3 UI elements that are exclusively used by player party members.
	for (size_t j = 0ui64; j < BATTLE_MAX_PARTY_SIZE; j++)
		uiElements[j] = new PartyUIElement();

	// Create the 8 UI elements for each possible enemy that can exist in a battle.
	for (size_t i = BATTLE_MAX_PARTY_SIZE; i < BATTLE_TOTAL_COMBATANTS; i++)
		uiElements[i] = new BattleUIElement();
}

void BattleUI::OnUserDestroy() {
	for (size_t i = 0ui64; i < BATTLE_TOTAL_COMBATANTS; i++)
		delete uiElements[i], uiElements[i] = nullptr;
}

void BattleUI::OnUserUpdate(float_t _deltaTime) {
	updateTimer -= _deltaTime;
	if (updateTimer > 0.0f)
		return;
	updateTimer = BATUI_UPDATE_INTERVAL;

	for (BattleUIElement* _element : uiElements)
		_element->OnUserUpdate();
}

void BattleUI::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	for (BattleUIElement* _element : uiElements)
		_element->OnUserRender(_engine);
}

void BattleUI::ActivateElement(Combatant* _combatant, size_t _index) {
	if (_index >= BATTLE_TOTAL_COMBATANTS || (uiElements[_index]->flags & FLAG_BATUI_ELEMENT_IN_USE))
		return;

	if (_index < BATTLE_MAX_PARTY_SIZE) {
		uiElements[_index]->ActivateElement(
			PARTY_UI_X, 
			PARTY_UI_Y + (PARTY_UI_SPACING_Y * totalPartyMembers),
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