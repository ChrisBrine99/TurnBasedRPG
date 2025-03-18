#ifndef BATTLE_CONTROLLER_HPP
#define BATTLE_CONTROLLER_HPP

#include "../Utils/GeneralMacros.hpp"
#include "../Utils/BattleMacros.hpp"

#include <array>
#include <vector>

class BattleMainMenu;
class BattlePartyInfoUI;
struct Combatant;

class BattleManager {
	// Singleton Initialization/Constructor Declaration
	INIT_SINGLETON_HPP(BattleManager)
	BattleManager();
public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(float_t _deltaTime);

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
	
public: // Publicly Accessible Utility Function Declarations
	void SetEncounterID(uint16_t _encounterID);
	uint16_t GetCombatantSpeed(Combatant* _combatant) const;

private: // Combatant Management Function Declarations
	void AddPlayerCombatant(size_t _partyIndex);
	void AddEnemyCombatant(uint16_t _enemyID);
	void RemoveCombatant(size_t _index);

public: // Accessible Member Variable Declarations
	BattleMainMenu* actionMenu;
	BattlePartyInfoUI* partyInfoUI;
	Combatant* curCombatant;

	std::vector<std::pair<uint16_t, uint8_t>> curItemRewards;
	uint32_t curMoneyReward;
	uint32_t curExpReward;

	float_t turnDelay;

	uint8_t curState;
	uint8_t nextState;
	uint8_t lastState;

private: // Hidden Member Variable Declarations 
	uint8_t curTurn;
	uint16_t encounterID;
	uint8_t curRound;
	uint8_t numCombatants;

	uint32_t flags;

	std::array<Combatant*, BATTLE_TOTAL_COMBATANTS> combatants;
	std::vector<size_t>	turnOrder;
};

#endif