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
#include "../UI/Battle/BattlePartyInfoUI.hpp"

BattleManager::BattleManager() :
	actionMenu(nullptr),
	partyInfoUI(nullptr),
	curCombatant(nullptr),
	curItemRewards(),
	curMoneyReward(0ui32),
	curExpReward(0ui32),
	turnDelay(0.0f),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID),
	curTurn(0ui8),
	encounterID(ID_INVALID),
	curRound(0ui8),
	numCombatants(0ui8),
	flags(0u),
	combatants(),
	turnOrder()
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
	for (Combatant* _c : combatants)
		delete _c, _c = nullptr;

	turnOrder.clear();
	turnOrder.shrink_to_fit();

	curItemRewards.clear();
	curItemRewards.shrink_to_fit();

	return true;
}

bool BattleManager::OnUserUpdate(float_t _deltaTime) {
	if (partyInfoUI != nullptr) { partyInfoUI->OnUserUpdate(_deltaTime); }

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
	case STATE_BATTLE_PLAYER_TURN:		return StatePlayerTurn(_deltaTime);
	case STATE_BATTLE_ENEMY_TURN:		return StateEnemyTurn(_deltaTime);
	case STATE_BATTLE_IS_ROUND_DONE:	return StateIsRoundFinished();
	case STATE_BATTLE_WIN:				return StateBattleWin();
	case STATE_BATTLE_LOSE:				return StateBattleLose();
	case STATE_BATTLE_ESCAPE:			return StateBattleEscape();
	case STATE_BATTLE_POST:				return StatePostBattle();
	case STATE_INVALID:					return true;
	}

	return false;
}

bool BattleManager::OnUserRender(float_t _deltaTime) {
	if (partyInfoUI == nullptr)
		return true;
	return partyInfoUI->OnUserRender(_deltaTime);
}

bool BattleManager::OnBeforeUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	return true;
}

void BattleManager::OnAfterUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	UPDATE_STATE(nextState);
}

bool BattleManager::StateInitializeBattle() {
	if (BATTLE_IS_ACTIVE)
		return true; // Prevents a battle from being initialized when one is already active.
	flags |= FLAG_BATTLE_ACTIVE;

	actionMenu = new BattleMainMenu();
	actionMenu->OnUserCreate();

	partyInfoUI = new BattlePartyInfoUI();
	partyInfoUI->OnUserCreate();

	// Attempt to fetch the relevant encounter data from within the data that was loaded on startup. The attempt to initialize
	// the battle will fail if the data returned is null.
	json& _encounterData = GET_SINGLETON(DataManager)->GetEncounterData(encounterID);
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
			std::cout << "Added enemy with ID " << uint32_t(_enemyID) << " to the battle." << std::endl;
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
	std::cout << "Turn Order: { ";
	for (size_t i : turnOrder)
		std::cout << i << " ";
	std::cout << "}" << std::endl;

	SET_NEXT_STATE(STATE_BATTLE_CHECK_TURN_TYPE);
	return true;
}

bool BattleManager::StateIsPlayerOrEnemyTurn() {
	curCombatant = combatants[turnOrder[curTurn]];

	if (COMBATANT_IS_PLAYER(curCombatant)) {
		SET_NEXT_STATE(STATE_BATTLE_PLAYER_TURN);
		actionMenu->PrepareForActivation(STATE_MENU_DEFAULT, curCombatant);
		return true;
	}

	SET_NEXT_STATE(STATE_BATTLE_ENEMY_TURN);
	return true;
}

bool BattleManager::StatePlayerTurn(float_t _deltaTime) {
	if (GET_SINGLETON(EngineCore)->GetKey(olc::SPACE).bPressed) {
		curCombatant->curHitpoints = uint16_t(std::rand() % curCombatant->maxHitpoints);
		curCombatant->curMagicpoints = uint16_t(std::rand() % curCombatant->maxMagicpoints);
	}
	return true;
}

bool BattleManager::StateEnemyTurn(float_t _deltaTime) {
	EnemyCharacter* _enemy = (EnemyCharacter*)curCombatant->character;
	if (typeid(*_enemy).hash_code() == typeid(EnemyCharacter).hash_code()) {
		SET_NEXT_STATE(STATE_BATTLE_IS_ROUND_DONE);
		_enemy->ExecuteAI(_deltaTime);
		return true;
	}
	std::cout << "Non-enemy Combatant attempted to execute an enemy turn. Terminating program now..." << std::endl;
	return false;
}

bool BattleManager::StateIsRoundFinished() {
	curTurn++;

	// The current round has finished, so reset the "curTurn" value back to 0 and increment the round number to reflect
	// that. Then, update the turn order for the next round of turns.
	if (curTurn == numCombatants) {
		SET_NEXT_STATE(STATE_BATTLE_SET_TURN_ORDER);
		curTurn = 0ui8;
		curRound++;
		return true;
	}

	Combatant* _combatants = nullptr;
	size_t _playersRemaining = 0ui64;
	size_t _enemiesRemaining = 0ui64;
	for (size_t i = 0ui64; i < turnOrder.size(); i++) {
		_combatants = combatants[turnOrder[i]];
		if (_combatants->curHitpoints == 0ui16) {
			RemoveCombatant(turnOrder[i]);
			i--; // Decrement by 1 to account for the current index being deleted.
			continue;
		}

		if (COMBATANT_IS_PLAYER(_combatants)) {
			_playersRemaining++;
			continue;
		}
		_enemiesRemaining++;
	}

	// Get the total number of player party members still in the battle. If the value is 0, the player has lost the battle.
	std::cout << "Allies Remaining: " << _playersRemaining << std::endl;
	if (_playersRemaining == 0ui64) {
		SET_NEXT_STATE(STATE_BATTLE_LOSE);
		return true;
	}

	// If there are still party members, count the total number of remaining enemies. If that value is 0, the battle is won.
	std::cout << "Enemies Remaining: " << _enemiesRemaining << std::endl;
	if (_enemiesRemaining == 0ui64) {
		SET_NEXT_STATE(STATE_BATTLE_WIN);
		return true;
	}

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

	curRound = 0ui8;
	curTurn = 0ui8;

	for (Combatant* _c : combatants)
		_c->flags = 0u;
	turnOrder.clear();
	curCombatant = nullptr;
	numCombatants = 0ui8;

	curItemRewards.clear();
	curMoneyReward = 0ui32;
	curExpReward = 0ui32;

	SET_NEXT_STATE(STATE_INVALID);
	return true;
}

void BattleManager::ExecuteSkill(Skill* _skill) {

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

void BattleManager::AddPlayerCombatant(size_t _partyIndex) {
	if (turnOrder.size() >= BATTLE_TOTAL_COMBATANTS)
		return;

	PlayerCharacter* _player = GET_SINGLETON(PartyManager)->GetActiveRosterMember(_partyIndex);
	if (_player == nullptr)
		return;

	for (size_t i = 0ui64; i < combatants.size(); i++) {
		if (COMBATANT_IS_ACTIVE(combatants[i]))
			continue;

		combatants[i]->ActivateCombatant(_player, FLAG_COMBATANT_PLAYER);
		partyInfoUI->ActivateElement(combatants[i], 320, 180 + int32_t(i * 15));
		turnOrder.push_back(i);
		numCombatants++;
		return;
	}
}

void BattleManager::AddEnemyCombatant(uint16_t _enemyID) {
	if (turnOrder.size() >= BATTLE_TOTAL_COMBATANTS)
		return;

	EnemyCharacter* _enemy = (EnemyCharacter*)GET_SINGLETON(DataManager)->GetCharacter(_enemyID);
	if (typeid(*_enemy).hash_code() != typeid(EnemyCharacter).hash_code())
		return;

	for (size_t i = 0ui64; i < BATTLE_TOTAL_COMBATANTS; i++) {
		if (COMBATANT_IS_ACTIVE(combatants[i]))
			continue;
		combatants[i]->ActivateCombatant(_enemy, 0u);
		turnOrder.push_back(i);
		numCombatants++;
		return;
	}
}

void BattleManager::RemoveCombatant(size_t _index) {
	combatants[_index]->flags = 0u; // Reset all flags

	for (size_t j = 0ui64; j < turnOrder.size(); j++){
		if (turnOrder[j] != _index)
			continue;

		turnOrder.erase(turnOrder.begin() + j);
		numCombatants--;
		return;
	}
}