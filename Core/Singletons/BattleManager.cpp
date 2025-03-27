#include "BattleManager.hpp"
INIT_SINGLETON_CPP(BattleManager)

#include "EngineCore.hpp"
#include "DataManager.hpp"
#include "MenuManager.hpp"
#include "PartyManager.hpp"
#include "../Structs/Battle/Combatant.hpp"
#include "../Structs/Battle/Skill.hpp"
#include "../Structs/Characters/EnemyCharacter.hpp"
#include "../UI/Menus/Battle/BattleMainMenu.hpp"
#include "../UI/Battle/BattleUI.hpp"
#include "../UI/Battle/BattleUIElement.hpp"
#include "../Utils/UtilityFunctions.hpp"

std::array<std::pair<int32_t, int32_t>, BATTLE_MAX_ENEMY_SIZE> BattleManager::enemyPositions = {
	std::make_pair(256i32, 64i32),		// First enemy combatant's position.
	std::make_pair(320i32, 64i32),		// Second enemy combatant's position.
	std::make_pair(256i32, 128i32),		// Third enemy combatant's position.
	std::make_pair(320i32, 128i32),		// Fourth enemy combatant's position.
	std::make_pair(384i32, 64i32),		// Fifth enemy combatant's position.
	std::make_pair(192i32, 64i32),		// Sixth enemy combatant's position.
	std::make_pair(384i32, 192i32),		// Seventh enemy combatant's position.
	std::make_pair(192i32, 192i32)		// Eighth enemy combatant's position.
};

BattleManager::BattleManager() :
	actionMenu(nullptr),
	battleUI(nullptr),
	curCombatant(nullptr),
	curItemRewards(),
	curMoneyReward(0ui32),
	curExpReward(0ui32),
	combatants(),
	turnOrder(),
	turnDelay(0.0f),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID),
	curTurn(0ui8),
	encounterID(ID_INVALID),
	curRound(0ui8),
	curSkillTarget(0ui8),
	flags(0u),
	skillToUse(nullptr)
{
	combatants.fill(nullptr); // Fill all indexes with nullptr to avoid unintended behavior before the array is populated.
	turnOrder.reserve(BATTLE_TOTAL_COMBATANTS); // Reserve the memory for the vector's maximum number of elements.
}

bool BattleManager::OnUserCreate() {
	for (size_t i = 0ui64; i < BATTLE_TOTAL_COMBATANTS; i++)
		combatants[i] = new Combatant();
	return true;
}

bool BattleManager::OnUserDestroy() {
	DESTROY_MENU(actionMenu, BattleMainMenu);
	if (battleUI) { delete battleUI, battleUI = nullptr; }

	for (Combatant* _c : combatants)
		delete _c, _c = nullptr;

	turnOrder.clear();
	turnOrder.shrink_to_fit();

	curItemRewards.clear();

	return true;
}

bool BattleManager::OnUserUpdate(float_t _deltaTime) {
	if (battleUI) { battleUI->OnUserUpdate(_deltaTime); }

	if (turnDelay > 0.0f) {
		turnDelay -= _deltaTime;
		if (turnDelay < 0.0f)
			turnDelay = 0.0f;
		return true;
	}

	switch (curState) {
	case STATE_BATTLE_INITIALIZE:		return StateInitializeBattle();
	case STATE_BATTLE_SET_TURN_ORDER:	return StateDetermineTurnOrder();
	case STATE_BATTLE_CHECK_TURN_TYPE:	return StateIsPlayerOrEnemyTurn();
	case STATE_BATTLE_PLAYER_TURN:		return StatePlayerTurn();
	case STATE_BATTLE_ENEMY_TURN:		return StateEnemyTurn(_deltaTime);
	case STATE_BATTLE_EXECUTE_SKILL:	return StateExecuteSkill();
	case STATE_BATTLE_IS_ROUND_DONE:	return StateIsRoundFinished();
	case STATE_BATTLE_WIN:				return StateBattleWin();
	case STATE_BATTLE_LOSE:				return StateBattleLose();
	case STATE_BATTLE_ESCAPE:			return StateBattleEscape();
	case STATE_BATTLE_POST:				return StatePostBattle();
	case STATE_INVALID:					return true;
	}

	return false;
}

bool BattleManager::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	if (battleUI) { battleUI->OnUserRender(_engine, _deltaTime); }
	return true;
}

bool BattleManager::OnBeforeUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	return true;
}

bool BattleManager::OnAfterUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	UPDATE_STATE(nextState);
	return true;
}

bool BattleManager::StateInitializeBattle() {
	if (BATTLE_IS_ACTIVE)
		return true; // Prevents a battle from being initialized when one is already active.
	flags |= FLAG_BATTLE_ACTIVE;

	actionMenu = CREATE_NEW_MENU(BattleMainMenu);
	battleUI = new BattleUI();
	battleUI->OnUserCreate();

	// Attempt to fetch the relevant encounter data from within the data that was loaded on startup. The attempt to initialize
	// the battle will fail if the data returned is null.
	json& _encounterData = GET_SINGLETON(DataManager)->GetEncounter(encounterID);
	if (_encounterData.is_null())
		return false;

	uint16_t _enemyID		= ID_INVALID;
	uint32_t _totalToSpawn	= 0u;
	uint8_t _spawnChance	= 0ui8;
	size_t _totalEnemies	= _encounterData[KEY_ENCOUNTER_ENEMIES].size();
	for (size_t i = 0ui64; i < _totalEnemies; i++) {
		// Cache all of these values within local variables so they don't need to constantly be retrieved when the spawn count 
		// loop is executed below.
		_enemyID		= _encounterData[KEY_ENCOUNTER_ENEMIES][i];
		_spawnChance	= _encounterData[KEY_ENCOUNTER_SPAWN_CHANCE][i];
		_totalToSpawn	= _encounterData[KEY_ENCOUNTER_SPAWN_COUNT][i];

		for (size_t ii = 0ui64; ii < _totalToSpawn; ii++) {
			// Generate a number between 0 and 255 to see if the enemy will be spawned for this encounter. If the enemy's spawn 
			// chance is equal to 255 (0xFF in hex) then the entire check is skipped and they're always added to the battle.
			if (_spawnChance != 0xFFui8 && _spawnChance < uint8_t(std::rand() % 0xFFui8))
				continue;
			AddEnemyCombatant(_enemyID);
		}
	}

	for (size_t j = 0ui64; j < PARTY_ACTIVE_MAX_SIZE; j++)
		AddPlayerCombatant(j);

	SET_NEXT_STATE(STATE_BATTLE_SET_TURN_ORDER);
	return true;
}

bool BattleManager::StateDetermineTurnOrder() {
	uint16_t _firstSpeed = 0ui16;
	uint16_t _secondSpeed = 0ui16;

	for (size_t i = 0ui64; i < turnOrder.size(); i++) {
		_firstSpeed = GetCombatantSpeed(combatants[turnOrder[i]]);

		for (size_t j = 0ui64; j < turnOrder.size(); j++) {
			if (i == j) continue; // Skip comparing the combatant against itself.

			_secondSpeed = GetCombatantSpeed(combatants[turnOrder[j]]);
			if (_firstSpeed < _secondSpeed || (_firstSpeed == _secondSpeed && std::rand() % 2 == 0)) {
				size_t _temp = turnOrder[j];
				turnOrder[j] = turnOrder[i];
				turnOrder[i] = _temp;
			}
		}
	}

	SET_NEXT_STATE(STATE_BATTLE_CHECK_TURN_TYPE);
	return true;
}

bool BattleManager::StateIsPlayerOrEnemyTurn() {
	size_t _index = turnOrder[curTurn];
	curCombatant = combatants[_index];

	if (_index < BATTLE_MAX_PARTY_SIZE) {
		SET_NEXT_STATE(STATE_BATTLE_PLAYER_TURN);
		actionMenu->PrepareForActivation(STATE_MENU_DEFAULT, curCombatant);
		return true;
	}

	SET_NEXT_STATE(STATE_BATTLE_ENEMY_TURN);
	return true;
}

bool BattleManager::StatePlayerTurn() {
	if (GET_SINGLETON(EngineCore)->GetKey(olc::SPACE).bPressed) {
		curCombatant->curHitpoints = std::rand() % curCombatant->maxHitpoints;
		curCombatant->curMagicpoints = std::rand() % curCombatant->maxMagicpoints;
	}
	return true;
}

bool BattleManager::StateEnemyTurn(float_t _deltaTime) {
	EnemyCharacter* _enemy = (EnemyCharacter*)curCombatant->character;
	if (typeid(*_enemy).hash_code() == typeid(EnemyCharacter).hash_code()) {
		SET_NEXT_STATE(STATE_BATTLE_IS_ROUND_DONE);
		_enemy->ExecuteAI();
		return true;
	}

	return false;
}

bool BattleManager::StateExecuteSkill() {
	if (skillToUse == nullptr || curSkillTarget >= targets.size())
		return false;

	size_t _index		= targets[curSkillTarget];
	Combatant* _target	= combatants[_index];
	skillToUse->ExecuteUseFunction(_target);
	turnDelay = 0.05f;

	if (_index >= PARTY_ACTIVE_MAX_SIZE)
		battleUI->uiElements[_index]->ShowElement(1.25f);

	curSkillTarget++;
	if (curSkillTarget == targets.size()) {
		SET_NEXT_STATE(STATE_BATTLE_IS_ROUND_DONE);
		skillToUse = nullptr;
		turnDelay = 0.5f;
		targets.clear();
	}
	return true;
}

void BattleManager::UpdateHitpoints(Combatant* _combatant, uint16_t _value) {
	if (_value > _combatant->curHitpoints) { 
		_combatant->curHitpoints = 0ui16;
		RemoveCombatant(_combatant);
		return;
	}
	_combatant->curHitpoints -= _value;
}

void BattleManager::UpdateMagicpoints(Combatant* _combatant, uint16_t _value) {
	if (_value > _combatant->curMagicpoints) {
		_combatant->curMagicpoints = 0ui16;
		return;
	}
	_combatant->curMagicpoints -= _value;
}

bool BattleManager::StateIsRoundFinished() {
	// Perform a check to see how many party members and enemies are left in the battle.
	size_t _playersRemaining	= 0ui64;
	size_t _enemiesRemaining	= 0ui64;
	size_t _numCombatants		= turnOrder.size();
	for (size_t i = 0ui64; i < _numCombatants; i++) {
		if (turnOrder[i] < BATTLE_MAX_PARTY_SIZE) {
			_playersRemaining++;
			continue;
		}
		_enemiesRemaining++;
	}

	// If the number of player party members remaining in the battle is zero the battle has been lost, and the battle 
	// will switch to its loss state to reflect that.
	if (_playersRemaining == 0ui64) {
		SET_NEXT_STATE(STATE_BATTLE_LOSE);
		return true;
	}

	// If the number of enemies remaining in the battle is zero the battle has been lost, and the battle  will switch 
	// to its win state to reflect that.
	if (_enemiesRemaining == 0ui64) {
		SET_NEXT_STATE(STATE_BATTLE_WIN);
		return true;
	}

	// If the battle should continue, increment the turn counter and then check if the value exceeds the current number 
	// of active combatants. If so, the round counter will increment and start the turn loop once again.
	curTurn++;
	if (curTurn >= _numCombatants) {
		SET_NEXT_STATE(STATE_BATTLE_SET_TURN_ORDER);
		curTurn = 0ui8;
		curRound++;
		return true;
	}

	// The battle hasn't finished in a win, loss, or escape, and the round hasn't finished; conitnue onto the next turn.
	SET_NEXT_STATE(STATE_BATTLE_CHECK_TURN_TYPE);
	return true;
}

bool BattleManager::StateBattleWin() {
	return true;
}

bool BattleManager::StateBattleLose() {
	return true;
}

bool BattleManager::StateBattleEscape() {
	return true;
}

bool BattleManager::StatePostBattle() {
	GET_SINGLETON(MenuManager)->DestroyMenu(actionMenu);
	delete battleUI, battleUI = nullptr;

	curRound = 0ui8;
	curTurn = 0ui8;

	for (Combatant* _c : combatants)
		_c->isActive = false;
	turnOrder.clear();
	curCombatant = nullptr;

	curItemRewards.clear();
	curMoneyReward = 0ui32;
	curExpReward = 0ui32;
	flags = 0ui32;

	SET_NEXT_STATE(STATE_INVALID);
	return true;
}

void BattleManager::ExecuteSkill(Skill* _skill) {
	if (skillToUse)
		return;

	if (_skill->hpCost > 0ui16) { UpdateHitpoints(curCombatant,   _skill->hpCost); }
	if (_skill->mpCost > 0ui16) { UpdateMagicpoints(curCombatant, _skill->mpCost); }

	skillToUse = _skill;
	curSkillTarget = 0ui8;
	SET_NEXT_STATE(STATE_BATTLE_EXECUTE_SKILL);
}

void BattleManager::SetEncounterID(uint16_t _encounterID) {
	if (BATTLE_IS_ACTIVE)
		return;
	encounterID = _encounterID;
	SET_NEXT_STATE(STATE_BATTLE_INITIALIZE);
}

uint16_t BattleManager::GetCombatantSpeed(Combatant* _combatant) const {
	int16_t _speedMod = _combatant->GetCurrentStatModifier(SPEED_MODIFIER) - 3i8;

	if (_speedMod > 0i16)		{ return uint16_t(_combatant->baseSpeed * (4ui16 + _speedMod) / 4ui16); }
	else if (_speedMod < 0i16)	{ return uint16_t(_combatant->baseSpeed * 4ui16 / (4ui16 - _speedMod)); }

	return _combatant->baseSpeed;
}

void BattleManager::AddToPlayerRewards(Combatant* _combatant) {
	EnemyCharacter* _enemy = (EnemyCharacter*)_combatant->character;
	if (typeid(*_enemy).hash_code() != typeid(EnemyCharacter).hash_code())
		return;

	curExpReward += _enemy->expReward;
	curMoneyReward += _enemy->moneyReward;

	for (auto& _data : _enemy->itemRewards) {
		if (_data.second < std::rand() % 0xFFui8)
			continue;

		if (curItemRewards.find(_data.first) != curItemRewards.end()) {
			curItemRewards[_data.first]++;
			continue;
		}
		curItemRewards[_data.first] = 1ui8;
	}
}

void BattleManager::AddPlayerCombatant(size_t _partyIndex) {
	PlayerCharacter* _player = GET_SINGLETON(PartyManager)->GetActiveRosterMember(_partyIndex);
	if (_player == nullptr)
		return;

	Combatant* _combatant = combatants[_partyIndex];
	if (_combatant->isActive)
		return; // Active member is already within the battle; don't bother activating them again

	_combatant->ActivateCombatant(_player);
	battleUI->ActivateElement(_combatant, _partyIndex);
	turnOrder.push_back(_partyIndex);
}

void BattleManager::AddEnemyCombatant(uint16_t _enemyID) {
	if (turnOrder.size() >= BATTLE_TOTAL_COMBATANTS)
		return;

	EnemyCharacter* _enemy = (EnemyCharacter*)GET_SINGLETON(DataManager)->GetCharacter(_enemyID);
	if (typeid(*_enemy).hash_code() != typeid(EnemyCharacter).hash_code())
		return;

	Combatant* _combatant = nullptr;
	for (size_t i = BATTLE_MAX_PARTY_SIZE; i < BATTLE_TOTAL_COMBATANTS; i++) {
		_combatant = combatants[i];
		if (_combatant->isActive)
			continue;

		_combatant->ActivateCombatant(_enemy);
		 battleUI->ActivateElement(_combatant, i);
		turnOrder.push_back(i);
		return;
	}
}

void BattleManager::RemoveCombatant(Combatant* _combatant, bool _defeatedByPlayer) {
	size_t _length = turnOrder.size();
	for (size_t i = 0ui64; i < _length; i++){
		if (combatants[turnOrder[i]] != _combatant)
			continue;

		if (turnOrder[i] >= BATTLE_MAX_PARTY_SIZE)
			AddToPlayerRewards(_combatant);

		turnOrder.erase(turnOrder.begin() + i);
		_combatant->isActive = false;
		return;
	}
}