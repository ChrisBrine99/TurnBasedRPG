#include "PlayerCharacter.hpp"

#include "../../singleton/DataManager.hpp"

PlayerCharacter::PlayerCharacter(uint16_t _id) :
	BaseCharacter(_id),
	curExperience(0ui32),
	nextLevelExperience(0Ui32),
	knownSkills(),
	equipment(0xFFFFui16)
{}

PlayerCharacter::~PlayerCharacter() {
	knownSkills.clear();
}

void PlayerCharacter::RewardExperience(uint32_t _amount) {
	curExperience += _amount;
	// Don't bother leveling up if the player has hit (Or somehow exceeded...) the level limit of 100 OR they haven't 
	// reached the required experience.
	if (level >= CHR_MAX_LEVEL || curExperience < nextLevelExperience)
		return; 

	while (curExperience >= nextLevelExperience) {
		level++;

		// Break out of the loop if level 100 has been hit; setting the required amount to 0 since the maximum has been 
		// achieved already.
		if (level == CHR_MAX_LEVEL) { 
			nextLevelExperience = 0Ui32;
			break;
		}
		
		// Calculate the experience requirement for the next level. Then, loop again to see if that amount has also been 
		// exceeded by the character' current experience.
		nextLevelExperience = DetermineRequiredExperience(level + 1);
	}

	// Update the base maximum hp and mp values to reflect the increased level.
	maxHitpointBase		= CalculateMaxBaseHitpoints();
	maxMagicpointBase	= CalculateMaxBaseMagicpoints();
}