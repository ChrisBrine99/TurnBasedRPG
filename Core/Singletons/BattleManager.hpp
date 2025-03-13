#ifndef BATTLE_CONTROLLER_HPP
#define BATTLE_CONTROLLER_HPP

#include "../Utils/GeneralMacros.hpp"
#include "../Utils/BattleMacros.hpp"

#include <vector>

struct Combatant;
class BattleManager {
	// Singleton Initialization/Constructor Declaration
	INIT_SINGLETON_HPP(BattleManager)
	BattleManager();
public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();
	bool OnUserUpdate(float_t _deltaTime);

	bool OnBeforeUserUpdate(float_t _deltaTime);
	void OnAfterUserUpdate(float_t _deltaTime);

private: // State Function Declarations
	bool StateInitializeBattle();
	bool StateDetermineTurnOrder();
	bool StateIsPlayerOrEnemyTurn();
	bool StatePlayerTurn(float_t _deltaTime);
	bool StateEnemyTurn(float_t _deltaTime);
	bool StateIsRoundFinished();
	bool StateBattleWin();
	bool StateBattleLose();
	bool StateBattleEscape();
	bool StatePostBattle();
	
public:
	inline uint16_t GetCombatantSpeed(Combatant* _combatant) const;

private: // Combatant Management Function Declarations
	void AddPlayerCombatant(size_t _partyIndex);
	void AddEnemyCombatant(uint16_t _enemyID);
	void RemoveCombatant(Combatant* _combatant);

public: // Accessible Variable Declarations
	uint8_t curState;
	uint8_t nextState;
	uint8_t lastState;

	uint16_t curRound;		// Keeps track of the current number of true "turns" that have happened in the current battle.
	uint8_t curTurn;		// This keeps track of the turns within those main "turns" that are called rounds in the code. Resets to 0 when the round value is incremented.

	std::vector<Combatant*> combatants;
	std::vector<size_t>		turnOrder;
	Combatant*				curCombatant;

	std::vector<std::pair<uint16_t, uint8_t>> curItemRewards;
	uint32_t curMoneyReward;
	uint32_t curExpReward;
};

#endif