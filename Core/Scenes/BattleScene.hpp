#ifndef BATTLE_SCENE_HPP
#define BATTLE_SCENE_HPP

#include "Scene.hpp"
#include "../Utils/GeneralMacros.hpp"
#include "../Utils/BattleMacros.hpp"

#include <array>
#include <vector>
#include <unordered_map>

class BattleMainMenu;
class BattleUI;
class EngineCore;
struct Combatant;
struct Skill;

class BattleScene : public Scene {
public: // Constructor/Destructor Declarations
	BattleScene();
	BattleScene(const BattleScene& _other) = delete;
	~BattleScene() = default;
public: // Main Engine Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(EngineCore* _engine, float_t _deltaTime) override;

	bool OnBeforeUserUpdate(float_t _deltaTime) override;
	bool OnAfterUserUpdate(float_t _deltaTime) override;

private: // State Function Declarations
	bool StateInitializeBattle();
	bool StateDetermineTurnOrder();
	bool StateIsPlayerOrEnemyTurn();
	bool StatePlayerTurn();
	bool StateEnemyTurn(float_t _deltaTime);
	bool StateExecuteSkill();
	bool StateIsRoundFinished();
	bool StateBattleWin();
	bool StateBattleLose();
	bool StateBattleEscape();
	bool StatePostBattle();

public: // Publicly Accessible Utility Function Declarations
	void ExecuteSkill(Skill* _skill);
	void UpdateHitpoints(Combatant* _combatant, uint16_t _value);
	void UpdateMagicpoints(Combatant* _combatant, uint16_t _value);

	void SetEncounterID(uint16_t _encounterID);
	uint16_t GetCombatantSpeed(Combatant* _combatant) const;

private:
	void AddToPlayerRewards(Combatant* _combatant);

public: // Combatant Management Function Declarations
	void AddPlayerCombatant(size_t _partyIndex);
	void AddEnemyCombatant(uint16_t _enemyID);
	void RemoveCombatant(Combatant* _combatant, bool _defeatedByPlayer = false);

public: // Accessible Member Variable Declarations
	static std::array<std::pair<int32_t, int32_t>, BATTLE_TOTAL_COMBATANTS> positions;

	BattleMainMenu* actionMenu;
	BattleUI* battleUI;
	Combatant* curCombatant;

	std::unordered_map<uint16_t, uint8_t> curItemRewards;
	uint32_t curMoneyReward;
	uint32_t curExpReward;

	std::array<Combatant*, BATTLE_TOTAL_COMBATANTS> combatants;
	std::vector<size_t>	turnOrder;
	std::vector<size_t> targets;

	float_t turnDelay;

	uint8_t curState;
	uint8_t nextState;
	uint8_t lastState;

private: // Hidden Member Variable Declarations 
	uint8_t		curTurn;
	uint16_t	encounterID;
	uint8_t		curRound;
	uint8_t		curSkillTarget;

	uint32_t	flags;

	Skill* skillToUse;
};

#endif