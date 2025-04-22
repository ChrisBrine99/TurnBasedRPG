#include "PlayerCharacter.hpp"

#include "../../singleton/DataManager.hpp"

PlayerCharacter::PlayerCharacter(uint16_t _id) :
	BaseCharacter(_id),
	curExperience(0ui32),
	nextLevelExperience(0Ui32),
	knownSkills() 
{}

PlayerCharacter::~PlayerCharacter() {
	knownSkills.clear();
}

void PlayerCharacter::RewardExperience(uint32_t _amount) {
	curExperience += _amount;
	if (level >= CHR_MAX_LEVEL || curExperience < nextLevelExperience)
		return;

	while (curExperience >= nextLevelExperience) {
		level++;
		if (level == CHR_MAX_LEVEL) {
			nextLevelExperience = 0Ui32;
			break;
		}
		nextLevelExperience = DetermineRequiredExperience(level + 1);
	}

	maxHitpointBase = CalculateMaxBaseHitpoints();
	maxMagicpointBase = CalculateMaxBaseMagicpoints();
}