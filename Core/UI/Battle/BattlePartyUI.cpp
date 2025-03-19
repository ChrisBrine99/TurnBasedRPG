#include "BattlePartyUI.hpp"

#include "../../Singletons/EngineCore.hpp"
#include "../../Utils/BattleMacros.hpp"

BattlePartyUI::BattlePartyUI() :
	xPos(0),
	yPos(0),
	uiElements()
{ // Reserve enough memory to contain three elements since that is the max number of party members that can participate in a battle at once.
	uiElements.reserve(BATTLE_MAX_PARTY_SIZE);
}

bool BattlePartyUI::OnUserCreate() {
	for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++)
		uiElements.push_back(PartyMemberUI()); // Initialize all three instaces upon creation of the UI.
	return true;
}

bool BattlePartyUI::OnUserUpdate(float_t _deltaTime) {
	for (PartyMemberUI& _element : uiElements) {
		if (!PMINFO_IS_OCCUPIED(_element))
			continue; // Skip over inactive UI elements in case the active party isn't full.

		_element.updateTimer -= _deltaTime;
		if (_element.updateTimer > 0.0f)
			continue;
		_element.updateTimer = GAME_UPDATE_INTERVAL;

		// Update the UI element's current values shown for the party member's current HP by having the shown value approach the target
		// value (The combatant's true hitpoint value) at a fixed interval of about 60 updates per second.
		if (_element.curHitpoints != _element.combatant->curHitpoints) {
			ValueSetLinear(_element.curHitpoints, _element.combatant->curHitpoints, 1ui16);
			UpdateOnScreenElements(_element.curHitpoints, _element.combatant->maxHitpoints, _element.hpBarWidth, _element.sCurHitpoints);
		}

		// Perform the same update as above, but for the combatant's current magicpoint and the value shown on the UI.
		if (_element.curMagicpoints != _element.combatant->curMagicpoints) {
			ValueSetLinear(_element.curMagicpoints, _element.combatant->curMagicpoints, 1ui16);
			UpdateOnScreenElements(_element.curMagicpoints, _element.combatant->maxMagicpoints, _element.mpBarWidth, _element.sCurMagicpoints);
		}
	}

	return true;
}

bool BattlePartyUI::OnUserRender(float_t _deltaTime) {
	(void)(_deltaTime);

	EngineCore* _engine = GET_SINGLETON(EngineCore);
	int32_t _xx = 0;
	int32_t _yy = 0;
	int32_t _index = -1;
	for (PartyMemberUI& _element : uiElements) {
		_index++;
		if (!PMINFO_IS_VISIBLE(_element))
			continue; // Skip over inactive UI elements in case the active party isn't full.

		_xx = _element.xPos; // The left side of the UI element will display the name and level of the party member.
		_yy = _element.yPos + (_index * 15);
		_engine->DrawString(_xx, _yy,		_element.sName,	COLOR_WHITE);
		_engine->DrawString(_xx + 120, _yy,	_element.sLevel, COLOR_LIGHT_GRAY);
		
		_xx += 172; // Offset from origin for the party member's HP information.
		_engine->DrawString(_xx, _yy, "HP " + _element.sCurHitpoints, COLOR_LIGHT_RED);
		if (_element.curHitpoints < _element.combatant->maxHitpoints)
			_engine->DrawLine(olc::vi2d(_xx + _element.hpBarWidth, _yy + 10), 
							  olc::vi2d(_xx + PMINFO_UI_BAR_WIDTH, _yy + 10),
							  COLOR_DARK_GRAY);
		if (_element.curHitpoints > 0ui16) // Don't display the HP bar if the party member has no remaining HP.
			_engine->DrawLine(olc::vi2d(_xx, _yy + 10), 
							  olc::vi2d(_xx + _element.hpBarWidth, _yy + 10), 
							  COLOR_DARK_RED);

		_xx += 56; // Additional offset for the party member's MP information.
		_engine->DrawString(_xx, _yy, "MP " + _element.sCurMagicpoints, COLOR_LIGHT_PURPLE);
		if (_element.curMagicpoints < _element.combatant->maxMagicpoints)
			_engine->DrawLine(olc::vi2d(_xx + _element.mpBarWidth, _yy + 10), 
							  olc::vi2d(_xx + PMINFO_UI_BAR_WIDTH, _yy + 10),
							  COLOR_DARK_GRAY);
		if (_element.curMagicpoints > 0ui16) // Don't display the MP bar if the party member has no remaining MP.
			_engine->DrawLine(olc::vi2d(_xx, _yy + 10), 
							  olc::vi2d(_xx + _element.mpBarWidth, _yy + 10), 
							  COLOR_DARK_PURPLE);
	}

	return true;
}

void BattlePartyUI::ActivateElement(Combatant* _combatant, int32_t _startX, int32_t _startY) {
	for (PartyMemberUI& _element : uiElements) {
		if (PMINFO_IS_OCCUPIED(_element))
			continue;

		// Populate the UI element's data with what is relevant within the combatant.
		_element.xPos				= _startX;
		_element.yPos				= _startY;
		_element.sName				= _combatant->character->name;
		_element.sLevel				= "Lv:" + std::to_string(_combatant->level);
		_element.flags				= FLAG_PMINFO_OCCUPIED | FLAG_PMINFO_VISIBLE;
		_element.curHitpoints		= 0ui16;
		_element.curMagicpoints		= 0ui16;
		_element.combatant			= _combatant;
		return;
	}
}

void BattlePartyUI::DeactivateElement(Combatant* _combatant) {
	for (PartyMemberUI& _element : uiElements) {
		if (_element.combatant != _combatant)
			continue;

		_element.combatant = nullptr;
		_element.flags = 0u;
		return;
	}
}

void BattlePartyUI::UpdateOnScreenElements(uint32_t _numerator, uint32_t _denominator, uint32_t& _barValueWidth, std::string& _barValueString) {
	_barValueWidth = uint32_t(_numerator / float_t(_denominator) * float_t(PMINFO_UI_BAR_WIDTH));
	_barValueString = std::to_string(_numerator);
}