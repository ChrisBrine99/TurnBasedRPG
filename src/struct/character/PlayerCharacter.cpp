#include "PlayerCharacter.hpp"

#include "../../singleton/DataManager.hpp"

PlayerCharacter::PlayerCharacter() :
	BaseCharacter(),
	curExperience(0ui32),
	nextLevelExperience(0Ui32),
	knownSkills() 
{ // By default, all player characters will utilize the same basic attack, which will then be altered by their equipped weapon.
	GET_SINGLETON(DataManager)->LoadSkillData(ID_PLAYER_BASIC_ATK_0);
	basicAttack = ID_PLAYER_BASIC_ATK_0;
}

void PlayerCharacter::RewardExperience(uint32_t _amount) {
	curExperience += _amount;
	if (level >= MAXIMUM_LEVEL || curExperience < nextLevelExperience)
		return;

	while (curExperience >= nextLevelExperience) {
		level++;
		if (level == MAXIMUM_LEVEL) {
			nextLevelExperience = 0Ui32;
			break;
		}
		nextLevelExperience = DetermineRequiredExperience(level + 1);
	}

	maxHitpointBase = CalculateMaxBaseHitpoints();
	maxMagicpointBase = CalculateMaxBaseMagicpoints();
}