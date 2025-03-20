#include "BattleEnemyUI.hpp"

#include "../../Singletons/EngineCore.hpp"
#include "../../Singletons/BattleManager.hpp"
#include "../../Utils/BattleMacros.hpp"

BattleEnemyUI::BattleEnemyUI() :
	uiElements(),
	totalActiveElements(0u)
{ // Reserve enough memory to contain eight elements since that is the max number of enemies that can participate in a battle at once.
	uiElements.reserve(BATTLE_MAX_ENEMY_SIZE);
	OnUserCreate();
}

bool BattleEnemyUI::OnUserCreate() {
	for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++)
		uiElements.push_back(EnemyUI()); // Initialize all eight instaces upon creation of the UI.
	return true;
}

bool BattleEnemyUI::OnUserUpdate(float_t _deltaTime) {
	uint16_t _curHitpoints		= 0ui16;
	uint16_t _trueCurHitpoints	= 0ui16;
	for (EnemyUI& _element : uiElements) {
		if (!ENINFO_IS_OCCUPIED(_element) || !ENINFO_IS_VISIBLE(_element))
			continue;

		// 
		_curHitpoints		= _element.curHitpoints;
		_trueCurHitpoints	= _element.combatant->curHitpoints;

		// 
		_element.visibilityTimer -= _deltaTime;
		if (_element.visibilityTimer < 0.0f) {
			_element.visibilityTimer = 0.0f;

			if (_trueCurHitpoints == 0ui16) {
				_element.combatant = nullptr;
				_element.flags = 0u;
				totalActiveElements--;
				continue;
			}

			_element.flags &= ~FLAG_ENINFO_VISIBLE;
			continue;
		}

		// 
		_element.updateTimer -= _deltaTime;
		if (_element.updateTimer > 0.0f || _curHitpoints == _trueCurHitpoints)
			continue;
		_element.updateTimer = GAME_UPDATE_INTERVAL;

		// 
		ValueSetLinear(_curHitpoints, _trueCurHitpoints, 1ui16);
		_element.hpBarWidth = uint32_t(_curHitpoints / float_t(_element.combatant->maxHitpoints) * float_t(ENINFO_HP_BAR_WIDTH));
		_element.curHitpoints = _curHitpoints;
	}

	return true;
}

bool BattleEnemyUI::OnUserRender(float_t _deltaTime) {
	(void)(_deltaTime);

	EngineCore* _engine = GET_SINGLETON(EngineCore);
	int32_t _index = -1;
	int32_t _totalOffsetX = ((totalActiveElements * 50) >> 1);
	int32_t _xPos = 0;
	int32_t _yPos = 0;
	for (EnemyUI& _element : uiElements) {
		_index++;
		if (!ENINFO_IS_VISIBLE(_element))
			continue; // Skip over invisible enemy UI elements.
		_xPos = 320 + _element.xPos - _totalOffsetX + (_index * 50);
		_yPos = 180 + _element.yPos - 60 + ((_index % 2) * 20);

		if (_element.curHitpoints < _element.combatant->maxHitpoints)
			_engine->DrawLine(olc::vi2d(_xPos + _element.hpBarWidth, _yPos + 10),
				olc::vi2d(_xPos + ENINFO_HP_BAR_WIDTH, _yPos + 10),
				COLOR_DARK_GRAY);
		if (_element.curHitpoints > 0ui16) // Don't display the HP bar if the enemy has no remaining HP.
			_engine->DrawLine(olc::vi2d(_xPos, _yPos + 10),
				olc::vi2d(_xPos + _element.hpBarWidth, _yPos + 10),
				COLOR_DARK_RED);
	}

	return true;
}

void BattleEnemyUI::ActivateElement(Combatant* _combatant) {
	for (EnemyUI& _element : uiElements) {
		if (ENINFO_IS_OCCUPIED(_element))
			continue;
		totalActiveElements++;

		// Populate the UI element's data with what is relevant within the combatant.
		_element.curHitpoints	= _combatant->maxHitpoints;
		_element.combatant		= _combatant;
		_element.flags			= FLAG_ENINFO_OCCUPIED;
		return;
	}
}

void BattleEnemyUI::DeactivateElement(Combatant* _combatant) {
	for (EnemyUI& _element : uiElements) {
		if (!ENINFO_IS_OCCUPIED(_element) || _element.combatant != _combatant)
			continue;
		totalActiveElements--;

		_element.combatant = nullptr;
		_element.flags = 0u;
		return;
	}
}

void BattleEnemyUI::ShowElement(Combatant* _combatant, float_t _time) {
	for (EnemyUI& _element : uiElements) {
		if (!ENINFO_IS_OCCUPIED(_element) || _element.combatant != _combatant)
			continue;

		_element.flags |= FLAG_ENINFO_VISIBLE;
		_element.visibilityTimer = _time;
		return;
	}
}