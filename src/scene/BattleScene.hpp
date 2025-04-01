#ifndef BATTLE_SCENE_HPP
#define BATTLE_SCENE_HPP

#include "Scene.hpp"
#include "../utility/GeneralMacros.hpp"
#include "../utility/BattleMacros.hpp"

#include <array>
#include <vector>

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
	void UpdateHitpoints(Combatant* _combatant, int16_t _value);
	void UpdateMagicpoints(Combatant* _combatant, int16_t _value);

	void SetEncounterID(uint16_t _encounterID);
	uint16_t GetCombatantSpeed(Combatant* _combatant) const;
	size_t GetCombatantIndex(Combatant* _combatant) const;
	size_t GetCurCombatantIndex() const;

private:
	void AddToPlayerRewards(Combatant* _combatant);

public: // Combatant Management Function Declarations
	void AddPlayerCombatant(size_t _partyIndex);
	void AddEnemyCombatant(uint16_t _enemyID);
	void RemoveCombatant(Combatant* _combatant, bool _defeatedByPlayer = false);

public: // Accessible Member Variable Declarations
	static std::array<std::pair<float_t, float_t>, BATTLE_TOTAL_COMBATANTS> positions;

	BattleMainMenu* actionMenu;
	BattleUI*		battleUI;
	Combatant*		curCombatant;

	std::vector<std::pair<uint16_t, uint8_t>> curItemRewards;
	uint32_t	curMoneyReward;
	uint32_t	curExpReward;

	std::array<Combatant*, BATTLE_TOTAL_COMBATANTS> combatants;
	std::vector<size_t>	turnOrder;
	std::vector<size_t> targets;

	float_t		turnDelay;

	uint8_t		curState;
	uint8_t		nextState;
	uint8_t		lastState;

	uint8_t		curSkillTarget;

private: // Hidden Member Variable Declarations 
	uint16_t	encounterID;
	uint8_t		curTurn;
	uint8_t		curRound;

	uint32_t	flags;

	Skill*		skillToUse;
};

#endif