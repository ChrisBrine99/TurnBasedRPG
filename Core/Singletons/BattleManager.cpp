#include "BattleManager.hpp"
INIT_SINGLETON_CPP(BattleManager)

#include "EngineCore.hpp"
#include "DataManager.hpp"
#include "MenuManager.hpp"
#include "PartyManager.hpp"
#include "../Structs/Battle/Combatant.hpp"
#include "../Structs/Characters/EnemyCharacter.hpp"
#include "../UI/Menus/Battle/BattleMainMenu.hpp"

BattleManager::BattleManager() :
	actionMenu(nullptr),
	curCombatant(nullptr),
	curItemRewards(),
	curMoneyReward(0ui32),
	curExpReward(0ui32),
	turnDelay(0.0f),
	curState(INVALID_STATE),
	nextState(INVALID_STATE),
	lastState(INVALID_STATE),
	curTurn(0ui8),
	encounterID(ID_INVALID),
	curRound(0ui16),
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
	case STATE_POST_BATTLE:				return StatePostBattle();
	case INVALID_STATE:					return true;
	}

	return false;
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
	actionMenu = new BattleMainMenu();
	actionMenu->OnUserCreate();
	actionMenu->SetFlags(FLAG_MENU_BLOCK_INPUT, false);

	//AddEnemyCombatant(ID_TEST_ENEMY);

	for (size_t i = 0ui64; i < PARTY_ACTIVE_MAX_SIZE; i++)
		AddPlayerCombatant(i);

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
	curCombatant = combatants[turnOrder[curTurn]];

	if (FLAG_IS_COMBATANT_PLAYER(curCombatant)) {
		SET_NEXT_STATE(STATE_BATTLE_PLAYER_TURN);
		actionMenu->PrepareForPlayerTurn();
		return true;
	}

	SET_NEXT_STATE(STATE_BATTLE_ENEMY_TURN);
	return true;
}

bool BattleManager::StatePlayerTurn(float_t _deltaTime) {
	return true;
}

bool BattleManager::StateEnemyTurn(float_t _deltaTime) {
	EnemyCharacter* _enemy = static_cast<EnemyCharacter*>(curCombatant->character);
	if (typeid(*_enemy) == typeid(EnemyCharacter)) {
		SET_NEXT_STATE(STATE_BATTLE_IS_ROUND_DONE);
		if (_enemy->battleAI != nullptr) // Enemy will do nothing if it doesn't have a valid AI function attached to it.
			_enemy->battleAI(_deltaTime);
		return true;
	}

	std::cout << "Non-enemy Combatant attempted to execute an enemy turn. Terminating program now..." << std::endl;
	return false;
}

bool BattleManager::StateIsRoundFinished() {
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
	curRound = 0ui16;
	curTurn = 0ui8;

	for (Combatant* _c : combatants)
		_c->flags = 0u;
	turnOrder.clear();
	curCombatant = nullptr;

	curItemRewards.clear();
	curMoneyReward = 0ui32;
	curExpReward = 0ui32;

	SET_NEXT_STATE(INVALID_STATE);
	return true;
}

void BattleManager::SetEncounterID(uint16_t _encounterID) {
	if (FLAG_IS_BATTLE_ACTIVE)
		return;

	flags |= FLAG_BATTLE_ACTIVE; // Always activate a battle when the encounterID is set to a value.
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
		if (FLAG_IS_COMBATANT_ACTIVE(combatants[i]))
			continue;
		combatants[i]->ActivateCombatant(_player, FLAG_COMBATANT_PLAYER);
		turnOrder.push_back(i);
		return;
	}
}

void BattleManager::AddEnemyCombatant(uint16_t _enemyID) {
	if (turnOrder.size() >= BATTLE_TOTAL_COMBATANTS)
		return;

	EnemyCharacter* _enemy = static_cast<EnemyCharacter*>(GET_SINGLETON(DataManager)->GetCharacter(_enemyID));
	if (typeid(*_enemy) != typeid(EnemyCharacter))
		return;

	for (size_t i = 0ui64; i < BATTLE_TOTAL_COMBATANTS; i++) {
		if (FLAG_IS_COMBATANT_ACTIVE(combatants[i]))
			continue;
		combatants[i]->ActivateCombatant(_enemy, 0u);
		turnOrder.push_back(i);
		return;
	}
}

void BattleManager::RemoveCombatant(size_t _index) {
	combatants[_index]->flags = 0u; // Reset all flags

	for (size_t j = 0ui64; j < turnOrder.size(); j++){
		if (turnOrder[j] != _index)
			continue;

		turnOrder.erase(turnOrder.begin() + j);
		return;
	}
}