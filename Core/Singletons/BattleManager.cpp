#include "BattleManager.hpp"
INIT_SINGLETON_CPP(BattleManager)

#include "EngineCore.hpp"
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
	

	SET_NEXT_STATE(STATE_BATTLE_SET_TURN_ORDER);
	return true;
}

bool BattleManager::StateDetermineTurnOrder() {


	//SET_NEXT_STATE(STATE_BATTLE_CHECK_TURN_TYPE);
	return true;
}

bool BattleManager::StateIsPlayerOrEnemyTurn() {
	if (curCombatant == nullptr)
		return false;

	if (FLAG_IS_PLAYER_TYPE(curCombatant)) {
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

	// TODO -- Add active party members to the battle.
}

void BattleManager::AddEnemyCombatant(uint16_t _enemyID) {
	if (combatants.size() >= BATTLE_MAX_ENEMY_SIZE + BATTLE_MAX_PARTY_SIZE)
		return;

}

void BattleManager::RemoveCombatant(size_t _index) {
	if (_index >= combatants.size())
		return;

	delete combatants[_index], combatants[_index] = nullptr;
	combatants.erase(combatants.begin() + _index);
	turnOrder.erase(turnOrder.begin() + _index);
}