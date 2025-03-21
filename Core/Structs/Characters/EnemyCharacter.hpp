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

	// An enemy character's default constructor. It simply populates all variables unique to an enemy (Money/exp rewards, item 
	// rewards, etc.) with their default values. On top of that, it calls the parent constructor to initialize all inherited
	// variables from that class.
	EnemyCharacter() :
		BaseCharacter(),
		basicAttack(ID_INVALID),
		moneyReward(0ui16),
		expReward(0ui16),
		itemRewards(),
		battleAI(nullptr) 
	{ // Reserve some memory to make room for the potential item rewards that an enemy can have access to upon its defeat. 
		itemRewards.reserve(3ui64);
	}

	// The enemy character's destructor which will simply clear all the data structures managed by it and also de-allocate
	// pointers to memory occupied by its member variables as required.
	~EnemyCharacter() {
		itemRewards.clear();
		battleAI = nullptr;
	}

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