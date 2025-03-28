#include "BattleUIElement.hpp"

#include "BattleUI.hpp"
#include "../../struct/battle/Combatant.hpp"

BattleUIElement::BattleUIElement() :
	flags(0ui32),
	x(0.0f),
	y(0.0f),
	curHitpoints(0ui16),
	curMagicpoints(0ui16),
	curHpBarWidth(0ui16),
	curMpBarWidth(0ui16),
	hpBarX(0.0f),
	hpBarY(0.0f),
	mpBarX(0.0f),
	mpBarY(0.0f),
	visibleTime(0.0f),
	combatant(nullptr) 
{}

void BattleUIElement::OnUserUpdate() {
	if (!BATUI_ELEMENT_IS_IN_USE || !BATUI_ELEMENT_IS_VISIBLE)
		return; // Skip over elements that are currently not in use or visible.

	// Only decrement the visibility time if the UI element is set to have its visibility determined by if the "visibleTime"
	// value is greater than 0.0f.
	if (BATUI_ELEMENT_USES_TIMER) {
		visibleTime -= BATUI_UPDATE_INTERVAL;
		if (visibleTime < 0.0f) {
			flags &= ~FLAG_BATUI_ELEMENT_VISIBLE;
			return;
		}
	}

	// Only update the hp bar's current width if the ui element is toggled to display the combatant's hitpoint bar.
	uint16_t _trueValue = combatant->curHitpoints;
	if (BATUI_ELEMENT_IS_HP_SHOWN && curHitpoints != _trueValue) {
		ValueSetLinear(curHitpoints, _trueValue, 1ui16);
		curHpBarWidth = uint16_t(curHitpoints / float_t(combatant->maxHitpoints) * BATUI_ELEMENT_BAR_WIDTH);
	}

	if (!BATUI_ELEMENT_IS_MP_SHOWN)
		return; // Don't bother with the mp value/bar stuff if the ui element doesn't need to show it.

	_trueValue = combatant->curMagicpoints;
	if (curMagicpoints != _trueValue) { // Does the same shift with the mp value as what happened with the hp value.
		ValueSetLinear(curMagicpoints, _trueValue, 1ui16);
		curMpBarWidth = uint16_t(curMagicpoints / float_t(combatant->maxMagicpoints) * BATUI_ELEMENT_BAR_WIDTH);
	}
}

void BattleUIElement::OnUserRender(EngineCore* _engine){
	if (!BATUI_ELEMENT_IS_IN_USE || !BATUI_ELEMENT_IS_VISIBLE)
		return; // Skip over elements that are currently disabled or invisible.

	olc::vf2d _barPosition	= { x + hpBarX, y + hpBarY };
	float_t _barWidth		= float_t(curHpBarWidth);
	if (BATUI_ELEMENT_IS_HP_SHOWN) { // Don't bother with the hp bar rendering if the ui element doesn't need to show it.
		if (curHitpoints < combatant->maxHitpoints)
			_engine->FillRectDecal(_barPosition, { BATUI_ELEMENT_BAR_WIDTH, BATUI_ElEMENT_BAR_HEIGHT }, COLOR_DARK_GRAY);
		_engine->FillRectDecal(_barPosition, { _barWidth, BATUI_ElEMENT_BAR_HEIGHT }, COLOR_DARK_RED);
	}

	if (!BATUI_ELEMENT_IS_MP_SHOWN)
		return; // Don't bother with the mp value/bar stuff if the ui element doesn't need to show it.

	_barPosition	= { x + mpBarX, y + mpBarY };
	_barWidth		= float_t(curMpBarWidth);
	if (curMagicpoints < combatant->maxMagicpoints)
		_engine->FillRectDecal(_barPosition, { BATUI_ELEMENT_BAR_WIDTH, BATUI_ElEMENT_BAR_HEIGHT }, COLOR_DARK_GRAY);
	_engine->FillRectDecal(_barPosition, { _barWidth, BATUI_ElEMENT_BAR_HEIGHT }, COLOR_DARK_PURPLE);
}

void BattleUIElement::ActivateElement(float_t _x, float_t _y, float_t _hpBarX, float_t _hpBarY, float_t _mpBarX, float_t _mpBarY, Combatant* _combatant, uint32_t _flags) {
	flags			= _flags;
	x				= _x;
	y				= _y;
	hpBarX			= _hpBarX;
	hpBarY			= _hpBarY;
	mpBarX			= _mpBarX;
	mpBarY			= _mpBarY;
	combatant		= _combatant;
	curHitpoints	= _combatant->curHitpoints;
	curMagicpoints	= _combatant->curMagicpoints;
	curHpBarWidth	= uint16_t(BATUI_ELEMENT_BAR_WIDTH);
	curMpBarWidth	= uint16_t(BATUI_ELEMENT_BAR_WIDTH);
}

void BattleUIElement::ShowElement(float_t _time) {
	flags |= FLAG_BATUI_ELEMENT_VISIBLE;
	visibleTime = _time;
}