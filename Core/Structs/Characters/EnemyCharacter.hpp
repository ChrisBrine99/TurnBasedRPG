#ifndef ENEMY_CHARACTER_HPP
#define ENEMY_CHARACTER_HPP

#include "BaseCharacter.hpp"

struct EnemyCharacter : public BaseCharacter {
	uint16_t									basicAttack;

	uint16_t									moneyReward;
	uint16_t									expReward;

	uint16_t									itemRewardWeight;
	std::vector<std::pair<uint16_t, uint8_t>>	itemRewards;

	void										(*battleAI)(float_t _deltaTime);

	// An enemy character's default constructor. It simply populates all variables unique to an enemy (Money/exp rewards, item 
	// rewards, etc.) with their default values. On top of that, it calls the parent constructor to initialize all inherited
	// variables from that class.
	EnemyCharacter() :
		BaseCharacter(),
		basicAttack(ID_INVALID),
		moneyReward(0ui16),
		expReward(0ui16),
		itemRewardWeight(0ui8),
		itemRewards(),
		battleAI(nullptr) 
	{ // Reserve some memory to make room for the potential item rewards that an enemy can have access to upon its defeat. 
		itemRewards.reserve(3ui64);
	}

	// An enemy character should never be copied from the master list of enemy data that it currently resides in throughout 
	// the game's execution. As such, the copy constructor is deleted to prevent any accidental copying.
	EnemyCharacter(EnemyCharacter& _other) = delete;
};

#endif