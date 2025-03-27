#include "PartyUIElement.hpp"

#include "../../Structs/Battle/Combatant.hpp"

void PartyUIElement::OnUserUpdate() {
	BattleUIElement::OnUserUpdate();
	if (!BATUI_ELEMENT_IS_IN_USE || !BATUI_ELEMENT_IS_VISIBLE)
		return;

	uint16_t _trueValue = combatant->curHitpoints;
	if (curHitpoints != _trueValue) { 
		sCurHitpoints = std::to_string(curHitpoints);
		sCurHitpointsWidth = float_t(sCurHitpoints.size() * 8.0f);
	}

	_trueValue = combatant->curMagicpoints;
	if (curMagicpoints != _trueValue) { 
		sCurMagicpoints = std::to_string(curMagicpoints);
		sCurMagicpointsWidth = float_t(sCurMagicpoints.size() * 8.0f);
	}
}

void PartyUIElement::OnUserRender(EngineCore* _engine) {
	BattleUIElement::OnUserRender(_engine);
	if (!BATUI_ELEMENT_IS_IN_USE || !BATUI_ELEMENT_IS_VISIBLE)
		return;

	olc::vf2d _textPosition = { float_t(x), float_t(y) };
	_engine->DrawStringDecal(_textPosition, sName, COLOR_WHITE);
	_textPosition.x += 120.0f; // Offset the level's position on the screen so it doesn't overlap the party member's name.
	_engine->DrawStringDecal(_textPosition, "Lv", COLOR_LIGHT_GRAY);
	_textPosition.x += 40.0f - sLevelWidth; // Align the level to the right of where it is displayed on the UI.
	_engine->DrawStringDecal(_textPosition, sLevel, COLOR_WHITE);

	// Displaying the numerical value for the party member's hitpoints above where the bar will be drawn.
	_textPosition.x = float_t(x + hpBarX);
	_engine->DrawStringDecal(_textPosition, "HP", COLOR_RED);
	_textPosition.x += 40.0f - sCurHitpointsWidth; // Shifts over so the numerical value doesn't overlap the "HP" when drawn above.
	_engine->DrawStringDecal(_textPosition, sCurHitpoints, COLOR_LIGHT_RED);

	// Displaying the numerical value for the part member's magicpoints above where the bar will be drawn.
	_textPosition.x = float_t(x + mpBarX);
	_engine->DrawStringDecal(_textPosition, "MP", COLOR_PURPLE);
	_textPosition.x += 40.0f - sCurMagicpointsWidth; // Shifts over so the numerical value doesn't overlap the "MP" when drawn above.
	_engine->DrawStringDecal(_textPosition, sCurMagicpoints, COLOR_LIGHT_PURPLE);
}

void PartyUIElement::ActivateElement(int32_t _x, int32_t _y, int32_t _hpBarX, int32_t _hpBarY, int32_t _mpBarX, int32_t _mpBarY, Combatant* _combatant, uint32_t _flags) {
	BattleUIElement::ActivateElement(_x, _y, _hpBarX, _hpBarY, _mpBarX, _mpBarY, _combatant, _flags);
	sName					= _combatant->character->name;
	sLevel					= std::to_string(_combatant->level);
	sCurHitpoints			= std::to_string(_combatant->curHitpoints);
	sCurMagicpoints			= std::to_string(_combatant->curMagicpoints);
	sLevelWidth				= float_t(sLevel.size() * 8.0f);
	sCurHitpointsWidth		= float_t(sCurHitpoints.size() * 8.0f);
	sCurMagicpointsWidth	= float_t(sCurMagicpoints.size() * 8.0f);
}