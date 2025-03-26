#include "BattleUI.hpp"
#include "PartyUIElement.hpp"
#include "../../Singletons/EngineCore.hpp"
#include "../../Singletons/BattleManager.hpp"

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

void BattleUI::OnUserRender(float_t _deltaTime) {
	EngineCore* _engine = GET_SINGLETON(EngineCore);
	for (BattleUIElement* _element : uiElements)
		_element->OnUserRender(_engine);
}

uint32_t BattleUI::ActivateElement(Combatant* _combatant) {
	if (COMBATANT_IS_PLAYER(_combatant)) {
		for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++) {
			if (!(uiElements[i]->flags & FLAG_BATUI_ELEMENT_IN_USE)) {
				uiElements[i]->ActivateElement(
					PARTY_UI_X, 
					PARTY_UI_Y + (PARTY_UI_SPACING_Y * totalPartyMembers),
					PARTY_UI_HP_BAR_X, 
					PARTY_UI_HP_BAR_Y, 
					_combatant, 
					PARTY_UI_MP_BAR_X, 
					PARTY_UI_MP_BAR_Y
				);
				uiElements[i]->flags |= FLAG_BATUI_ELEMENT_IN_USE | FLAG_BATUI_ELEMENT_MP_SHOWN | FLAG_BATUI_ELEMENT_VISIBLE;
				totalPartyMembers++;
				return uint32_t(i);
			}
		}

		return uint32_t(BATTLE_MAX_PARTY_SIZE);
	}

	for (size_t i = BATTLE_MAX_PARTY_SIZE; i < BATTLE_TOTAL_COMBATANTS; i++) {
		if (!(uiElements[i]->flags & FLAG_BATUI_ELEMENT_IN_USE)) {
			auto& _coordinates = BattleManager::enemyPositions[i - BATTLE_MAX_PARTY_SIZE];
			uiElements[i]->ActivateElement(
				_coordinates.first,
				_coordinates.second,
				8i32,
				32i32,
				_combatant
			);
			uiElements[i]->flags |= FLAG_BATUI_ELEMENT_IN_USE | FLAG_BATUI_ELEMENT_USE_TIMER;
			return uint32_t(i);
		}
	}

	return uint32_t(BATTLE_MAX_PARTY_SIZE);
}

void BattleUI::DeactivateElement(uint32_t _element) {
	if (_element >= uint32_t(BATTLE_TOTAL_COMBATANTS))
		return;
	uiElements[_element]->flags = 0i32;
}

void BattleUI::ShowElement(uint32_t _element, float_t _time) {
	if (_element >= uint32_t(BATTLE_TOTAL_COMBATANTS))
		return;
	uiElements[_element]->ShowElement(_time);
}