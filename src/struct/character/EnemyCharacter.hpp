#ifndef ENEMY_CHARACTER_HPP
#define ENEMY_CHARACTER_HPP

#include "BaseCharacter.hpp"

class BattleScene;

struct EnemyCharacter : public BaseCharacter {
	uint16_t moneyReward;
	uint16_t expReward;

	std::vector<std::pair<uint16_t, uint8_t>>	itemRewards;

	// The function that will be called during the enemy's turn in battle.
	void (EnemyCharacter::*battleAI)(BattleScene*);

public: // Constructor/Destructor Declarations
	EnemyCharacter();
	EnemyCharacter(const EnemyCharacter& _other) = delete;
	~EnemyCharacter() = default;

public: // Utility Function Definitions

	// The function that is responsible for executing the Enemy's ai function. If the value in "battleAI" is nullptr, this 
	// function will do nothing and the enemy's turn is completely skipped as a result.
	inline void ExecuteAI(BattleScene* _scene) {
		if (battleAI == nullptr)
			return;
		((*this).*(this->battleAI))(_scene);
	}

public: // Enemy AI Function Declarations
	void EnemySimpleAI(BattleScene* _scene);
};

#endif