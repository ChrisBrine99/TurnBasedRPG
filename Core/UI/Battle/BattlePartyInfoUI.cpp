#include "BattlePartyInfoUI.hpp"

#include "../../Singletons/EngineCore.hpp"
#include "../../Utils/BattleMacros.hpp"

BattlePartyInfoUI::BattlePartyInfoUI() :
	xPos(0),
	yPos(0),
	flags(0u),
	uiElements()
{ // Reserve enough memory to contain three elements since that is the max number of party members that can participate in a battle at once.
	uiElements.reserve(BATTLE_MAX_PARTY_SIZE);
}

bool BattlePartyInfoUI::OnUserCreate() {
	for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++)
		uiElements.push_back(PartyMemberUI()); // Initialize all three instaces upon creation of the UI.
	return true;
}

bool BattlePartyInfoUI::OnUserUpdate(float_t _deltaTime) {
	for (PartyMemberUI& _element : uiElements) {
		if (!FLAG_IS_PMINFO_OCCUPIED(_element))
			continue; // Skip over inactive UI elements in case the active party isn't full.

		_element.sUpdateTimer -= _deltaTime;
		if (_element.sUpdateTimer > 0.0f)
			continue;
		_element.sUpdateTimer = 0.01f;

		_element.UpdateHitpointUI();
		_element.UpdateMagicpointUI();
	}

	return true;
}

bool BattlePartyInfoUI::OnUserRender(float_t _deltaTime) {
	(void)(_deltaTime);

	EngineCore* _engine = GET_SINGLETON(EngineCore);
	int32_t _xx = 0;
	int32_t _yy = 0;
	for (PartyMemberUI& _element : uiElements) {
		if (!FLAG_IS_PMINFO_VISIBLE(_element))
			continue; // Skip over inactive UI elements in case the active party isn't full.

		_xx = _element.xPos; // The left side of the UI element will display the name and level of the party member.
		_yy = _element.yPos;
		_engine->DrawString(_xx, _yy,		_element.sName,	COLOR_WHITE);
		_engine->DrawString(_xx + 120, _yy,	_element.sLevel, COLOR_LIGHT_GRAY);
		
		_xx += 172; // Offset from origin for the party member's HP information.
		_engine->DrawString(_xx, _yy, "HP " + _element.sCurHitpoints, COLOR_LIGHT_RED);
		_engine->DrawLine(olc::vi2d(_xx, _yy + 10), olc::vi2d(_xx + PMEMBER_UI_BAR_WIDTH, _yy + 10), COLOR_DARK_GRAY);
		if (_element.combatant->curHitpoints > 0ui16) // Don't display the HP bar if the party member has no remaining HP.
			_engine->DrawLine(olc::vi2d(_xx, _yy + 10), olc::vi2d(_xx + _element.hpBarWidth, _yy + 10), COLOR_DARK_RED);

		_xx += 56; // Additional offset for the party member's MP information.
		_engine->DrawString(_xx, _yy, "MP " + _element.sCurMagicpoints, COLOR_LIGHT_PURPLE);
		_engine->DrawLine(olc::vi2d(_xx, _yy + 10), olc::vi2d(_xx + PMEMBER_UI_BAR_WIDTH, _yy + 10), COLOR_DARK_GRAY);
		if (_element.combatant->curMagicpoints > 0ui16) // Don't display the MP bar if the party member has no remaining MP.
			_engine->DrawLine(olc::vi2d(_xx, _yy + 10), olc::vi2d(_xx + _element.mpBarWidth, _yy + 10), COLOR_DARK_PURPLE);
	}

	return true;
}

void BattlePartyInfoUI::ActivateElement(Combatant* _combatant, int32_t _startX, int32_t _startY) {
	for (PartyMemberUI& _element : uiElements) {
		if (FLAG_IS_PMINFO_OCCUPIED(_element))
			continue;

		// Populate the UI element's data with what is found within the combatant.
		_element.xPos				= _startX;
		_element.yPos				= _startY;
		_element.sName				= _combatant->character->name;
		_element.sLevel				= "Lv:" + std::to_string(_combatant->level);
		_element.flags				= FLAG_PMINFO_OCCUPIED | FLAG_PMINFO_VISIBLE;
		_element.targetHitpoints	= _combatant->curHitpoints;
		_element.targetMagicpoints	= _combatant->curMagicpoints;
		_element.combatant			= _combatant;
		return;
	}
}

void BattlePartyInfoUI::DeactivateElement(Combatant* _combatant) {
	for (PartyMemberUI& _element : uiElements) {
		if (_element.combatant != _combatant)
			continue;

		_element.combatant = nullptr;
		_element.flags = 0u;
		return;
	}
}

void BattlePartyInfoUI::UpdateElement(Combatant* _combatant) {
	for (PartyMemberUI& _element : uiElements) {
		if (_element.combatant != _combatant)
			continue;

		_element.targetHitpoints = _combatant->curHitpoints;
		_element.targetMagicpoints = _combatant->curMagicpoints;
		return;
	}
}