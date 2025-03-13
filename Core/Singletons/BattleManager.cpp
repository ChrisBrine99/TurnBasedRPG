#include "BattleManager.hpp"
INIT_SINGLETON_CPP(BattleManager)

#include "EngineCore.hpp"
#include "PartyManager.hpp"
#include "DataManager.hpp"
#include "../Structs/Battle/Combatant.hpp"
#include "../Structs/Characters/EnemyCharacter.hpp"

BattleManager::BattleManager() :
	curState(INVALID_STATE),
	nextState(INVALID_STATE),
	lastState(INVALID_STATE),
	curRound(0ui16),
	curTurn(0ui8),
	combatants(),
	turnOrder(),
	curCombatant(nullptr),
	curItemRewards(),
	curMoneyReward(0ui32),
	curExpReward(0ui32)
{ // Reserve enough memory for the maximum possible number of enemies/party members that can be in a single given battle at once.
	combatants.reserve(BATTLE_MAX_ENEMY_SIZE + BATTLE_MAX_PARTY_SIZE);
	turnOrder.reserve(BATTLE_MAX_ENEMY_SIZE + BATTLE_MAX_PARTY_SIZE);
}

bool BattleManager::OnUserCreate() {
	SET_NEXT_STATE(STATE_BATTLE_INITIALIZE);
	return true;
}

bool BattleManager::OnUserDestroy() {
	if (combatants.size() > 0ui64) {
		for (Combatant* _combatant : combatants)
			delete _combatant, _combatant = nullptr;
		combatants.clear();
		combatants.shrink_to_fit();
	}

	turnOrder.clear();
	turnOrder.shrink_to_fit();

	curItemRewards.clear();
	curItemRewards.shrink_to_fit();

	return true;
}

bool BattleManager::OnUserUpdate(float_t _deltaTime) {
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
	AddEnemyCombatant(ID_TEST_ENEMY);

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

	for (Combatant* _combatant : combatants)
		delete _combatant, _combatant = nullptr;

	combatants.clear();
	turnOrder.clear();
	curCombatant = nullptr;

	curItemRewards.clear();
	curMoneyReward = 0ui32;
	curExpReward = 0ui32;

	SET_NEXT_STATE(INVALID_STATE);
	return true;
}

void BattleManager::AddPlayerCombatant(size_t _partyIndex) {
	if (combatants.size() >= BATTLE_MAX_ENEMY_SIZE + BATTLE_MAX_PARTY_SIZE)
		return;

	PlayerCharacter* _player = GET_SINGLETON(PartyManager)->GetActiveRosterMember(_partyIndex);
	if (_player == nullptr)
		return;

	combatants.push_back(new Combatant(_player, FLAG_COMBATANT_PLAYER));
	turnOrder.push_back(combatants.size() - 1ui64);
}

void BattleManager::AddEnemyCombatant(uint16_t _enemyID) {
	if (combatants.size() >= BATTLE_MAX_ENEMY_SIZE + BATTLE_MAX_PARTY_SIZE)
		return;

	EnemyCharacter* _enemy = static_cast<EnemyCharacter*>(GET_SINGLETON(DataManager)->GetCharacter(_enemyID));
	if (typeid(*_enemy) == typeid(EnemyCharacter)) {
		combatants.push_back(new Combatant(_enemy));
		turnOrder.push_back(combatants.size() - 1ui64);
	}
}

void BattleManager::RemoveCombatant(Combatant* _combatant) {
	size_t _index = 0ui64;
	for (Combatant* c : combatants) {
		if (c == _combatant) {
			delete c, c = nullptr;
			combatants.erase(combatants.begin() + _index);
			break;
		}
		_index++;
	}

	for (size_t j = 0ui64; j < turnOrder.size(); j++){
		if (turnOrder[j] != _index)
			continue;

		turnOrder.erase(turnOrder.begin() + j);
		break;
	}
}

inline uint16_t BattleManager::GetCombatantSpeed(Combatant* _combatant) const {
	int16_t _speedMod = _combatant->GetCurrentStatModifier(SPEED_MODIFIER) - 3i8;

	if (_speedMod > 0i16)		{ return uint16_t(_combatant->baseSpeed * (4ui16 + _speedMod) / 4ui16); }
	else if (_speedMod < 0i16)	{ return uint16_t(_combatant->baseSpeed * 4ui16 / (4ui16 - _speedMod)); }

	return _combatant->baseSpeed;
}