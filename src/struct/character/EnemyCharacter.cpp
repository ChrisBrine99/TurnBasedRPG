#include "EnemyCharacter.hpp"

#include "../../scene/BattleScene.hpp"
#include "../../singleton/DataManager.hpp"
#include "../battle/Combatant.hpp"

EnemyCharacter::EnemyCharacter(uint16_t _id) :
	BaseCharacter(_id),
	moneyReward(0ui16),
	expReward(0ui16),
	itemRewards(),
	battleAI(nullptr) 
{ // Reserve some memory to make room for the potential item rewards that an enemy can have access to upon its defeat. 
	itemRewards.reserve(3ui64);
}

EnemyCharacter::~EnemyCharacter() {
	itemRewards.clear();
}

void EnemyCharacter::EnemySimpleAI(BattleScene* _scene) {
	auto& _combatants = _scene->combatants;
	for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++) {
		if (_combatants[i]->isActive && (std::rand() % 2 == 0)) {
			_scene->targets.push_back(i);
			_scene->ExecuteActiveSkill(basicAttack);
			return;
		}
	}
}