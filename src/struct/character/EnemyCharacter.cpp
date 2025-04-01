#include "EnemyCharacter.hpp"

#include "../../scene/BattleScene.hpp"
#include "../../singleton/DataManager.hpp"

EnemyCharacter::EnemyCharacter() :
	BaseCharacter(),
	moneyReward(0ui16),
	expReward(0ui16),
	itemRewards(),
	battleAI(nullptr) 
{ // Reserve some memory to make room for the potential item rewards that an enemy can have access to upon its defeat. 
	itemRewards.reserve(3ui64);
}

void EnemyCharacter::EnemySimpleAI(BattleScene* _scene) {
	Skill* _skill = GET_SINGLETON(DataManager)->GetSkill(basicAttack);
	_scene->targets.push_back(0ui16);
	_scene->ExecuteSkill(_skill);
}