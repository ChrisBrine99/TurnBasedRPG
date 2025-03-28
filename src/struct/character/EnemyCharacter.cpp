#include "EnemyCharacter.hpp"

#include <iostream>

EnemyCharacter::EnemyCharacter() :
	BaseCharacter(),
	basicAttack(ID_INVALID),
	moneyReward(0ui16),
	expReward(0ui16),
	itemRewards(),
	battleAI(nullptr) 
{ // Reserve some memory to make room for the potential item rewards that an enemy can have access to upon its defeat. 
	itemRewards.reserve(3ui64);
}

void EnemyCharacter::EnemySimpleAI() {
	std::cout << "ENEMY AI FUNCTION CALLED" << std::endl;
}