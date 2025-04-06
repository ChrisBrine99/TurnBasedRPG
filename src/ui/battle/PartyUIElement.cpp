#include "PartyUIElement.hpp"

#include "../../struct/battle/Combatant.hpp"
#include "../../scene/BattleScene.hpp"

PartyUIElement::PartyUIElement() :
	BattleUIElement(),
	sceneRef(nullptr),
	sName("N/A"),
	sLevel("0"),
	sCurHitpoints("0"),
	sCurMagicpoints("0"),
	sLevelWidth(8ui16),
	sCurHitpointsWidth(8ui16),
	sCurMagicpointsWidth(8ui16) 
{}

void PartyUIElement::OnUserUpdate() {
	uint16_t _prevHitpoints		= curHitpoints;
	uint16_t _prevMagicpoints	= curMagicpoints;

	BattleUIElement::OnUserUpdate();
	if (!BATUI_ELEMENT_IS_IN_USE || !BATUI_ELEMENT_IS_VISIBLE)
		return;

	uint16_t _trueValue = combatant->curHitpoints;
	if (_prevHitpoints != _trueValue) {
		sCurHitpoints = std::to_string(curHitpoints);
		sCurHitpointsWidth = uint16_t(sCurHitpoints.size() * 8ui16);
	}

	_trueValue = combatant->curMagicpoints;
	if (_prevMagicpoints != _trueValue) {
		sCurMagicpoints = std::to_string(curMagicpoints);
		sCurMagicpointsWidth = uint16_t(sCurMagicpoints.size() * 8ui16);
	}
}

void PartyUIElement::OnUserRender(EngineCore* _engine) {
	BattleUIElement::OnUserRender(_engine);
	if (!BATUI_ELEMENT_IS_IN_USE || !BATUI_ELEMENT_IS_VISIBLE)
		return;

	if (combatant == sceneRef->curCombatant)
		_engine->DrawString( x - 16i32, y, ">", COLOR_LIGHT_YELLOW);

	olc::Pixel _color	= COLOR_WHITE;
	int32_t _offsetX	= x;
	if (curHitpoints / float_t(combatant->maxHitpoints) < 0.25f)
		_color = COLOR_RED;
	_engine->DrawString(_offsetX, y, sName, _color);
	_offsetX += 120i32; // Offset the level's position on the screen so it doesn't overlap the party member's name.
	_engine->DrawString(_offsetX, y, "Lv", COLOR_LIGHT_GRAY);
	_offsetX += 40i32 - int32_t(sLevelWidth); // Align the level to the right of where it is displayed on the UI.
	_engine->DrawString(_offsetX, y, sLevel, COLOR_WHITE);

	// Displaying the numerical value for the party member's hitpoints above where the bar will be drawn.
	_offsetX = x + hpBarX;
	_engine->DrawString(_offsetX, y, "HP", COLOR_RED);
	_offsetX += 40i32 - int32_t(sCurHitpointsWidth); // Shifts over so the numerical value doesn't overlap the "HP" when drawn above.
	_engine->DrawString(_offsetX, y, sCurHitpoints, COLOR_LIGHT_RED);

	// Displaying the numerical value for the part member's magicpoints above where the bar will be drawn.
	_offsetX = x + mpBarX;
	_engine->DrawString(_offsetX, y, "MP", COLOR_PURPLE);
	_offsetX += 40i32 - int32_t(sCurMagicpointsWidth); // Shifts over so the numerical value doesn't overlap the "MP" when drawn above.
	_engine->DrawString(_offsetX, y, sCurMagicpoints, COLOR_LIGHT_PURPLE);
}

void PartyUIElement::ActivateElement(int32_t _x, int32_t _y, int32_t _hpBarX, int32_t _hpBarY, int32_t _mpBarX, int32_t _mpBarY, Combatant* _combatant, uint32_t _flags, BattleScene* _sceneRef) {
	BattleUIElement::ActivateElement(_x, _y, _hpBarX, _hpBarY, _mpBarX, _mpBarY, _combatant, _flags);
	sceneRef				= _sceneRef;
	sName					= _combatant->character->name;
	sLevel					= std::to_string(_combatant->level);
	sCurHitpoints			= std::to_string(_combatant->curHitpoints);
	sCurMagicpoints			= std::to_string(_combatant->curMagicpoints);
	sLevelWidth				= uint16_t(sLevel.size() * 8i32);
	sCurHitpointsWidth		= uint16_t(sCurHitpoints.size() * 8i32);
	sCurMagicpointsWidth	= uint16_t(sCurMagicpoints.size() * 8i32);
}