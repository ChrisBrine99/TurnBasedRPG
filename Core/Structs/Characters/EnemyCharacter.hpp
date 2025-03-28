#ifndef ENEMY_CHARACTER_HPP
#define ENEMY_CHARACTER_HPP

#include "BaseCharacter.hpp"

#include <unordered_map>

struct EnemyCharacter : public BaseCharacter {
	uint16_t									basicAttack;

	uint16_t									moneyReward;
	uint16_t									expReward;

	std::unordered_map<uint16_t, uint8_t>		itemRewards;

	void										(EnemyCharacter::*battleAI)();

public: // Constructor/Destructor Declarations
	EnemyCharacter();
	EnemyCharacter(const EnemyCharacter& _other) = delete;
	~EnemyCharacter() = default;

public: // Utility Function Definitions

	// The function that is responsible for executing the Enemy's ai function. If the value in "battleAI" is nullptr, this 
	// function will do nothing and the enemy's turn is completely skipped as a result.
	inline void ExecuteAI() {
		if (battleAI == nullptr)
			return;
		((*this).*(this->battleAI))();
	}

public: // Enemy AI Function Declarations
	void EnemySimpleAI();
};

#endif