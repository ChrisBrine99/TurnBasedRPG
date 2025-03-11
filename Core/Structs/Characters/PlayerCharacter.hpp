#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

#include "BaseCharacter.hpp"

struct PlayerCharacter : public BaseCharacter {
	uint32_t						curExperience;
	uint32_t						nextLevelExperience;

	std::vector<uint16_t>			knownSkills;

	// Default contstructor that calls the BaseCharacter constructor to initialize every variable inherited by it, and then 
	// the variables and data structures unique to this child class will also be initialized after that.
	PlayerCharacter() : 
		BaseCharacter(),
		curExperience(0ui32),
		nextLevelExperience(0Ui32),
		knownSkills()
	{}

	// 
	PlayerCharacter(PlayerCharacter& _other) {
		level					= _other.level;

		curHitpoints			= _other.curHitpoints;
		curMagicpoints			= _other.curMagicpoints;

		maxHitpointBase			= _other.maxHitpointBase;
		maxHitpointBonus		= _other.maxHitpointBonus;
		maxHitpointMultiplier	= _other.maxHitpointMultiplier;

		maxMagicpointBase		= _other.maxMagicpointBase;
		maxMagicpointBonus		= _other.maxMagicpointBonus;
		maxMagicpointMultiplier = _other.maxMagicpointMultiplier;

		curExperience			= _other.curExperience;
		nextLevelExperience		= _other.nextLevelExperience;

		for (size_t a = 0ui64; a < STAT_COUNT; a++) {
			statBase[a]			= _other.statBase[a];
			statBonus[a]		= _other.statBonus[a];
			statMultiplier[a]	= _other.statMultiplier[a];
		}

		size_t _length = std::min(PLAYER_SKILL_LIMIT, _other.activeSkills.size());
		for (size_t b = 0ui64; b < _length; b++)
			activeSkills.push_back(_other.activeSkills[b]);

		for (size_t c = 0ui64; c < _other.knownSkills.size(); c++)
			knownSkills.push_back(_other.knownSkills[c]);
	}

	// Adds the given amount to the player character's current experience value. Then it checks to see if that value exceeds
	// the current requirement to reach the next level. If so, a loop will begin that increases their level until the 
	// calculated requirement exceeds the current experience value OR their level reaches 100. The character's max HP and MP 
	// values are updated to match the new level value after the loop has completed execution.
	void RewardExperience(uint32_t _amount) {
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

	// Returns a value for the experience required in order for the player character to increase their current level by one. 
	// Exceptions to the formula used are when the level value is 0, 1, or any value above 100--where a 0 is returned instead.
	inline uint32_t DetermineRequiredExperience(uint8_t _level) {
		if (_level <= 1Ui8 || _level > MAXIMUM_LEVEL)
			return 0U;
		return uint32_t(1.2f * float_t(_level * _level * _level));
	}

	// Contains the formula that determines a playable character's maximum HP value, which is affected by both the level and
	// current endurance stat.
	inline uint16_t CalculateMaxBaseHitpoints() {
		return uint16_t(level * 3ui16) + uint16_t(GetCurrentStatTotal(STAT_ENDURANCE) * 4ui16) + MINIMUM_PLAYER_HP_AND_MP;
	}

	// Contains the formula that determines a playable character's maximum MP value, which is affected by both the level and
	// current endurance stat.
	inline uint16_t CalculateMaxBaseMagicpoints() {
		return uint16_t(level * 3ui16) + uint16_t(GetCurrentStatTotal(STAT_MAGIC) * 4ui16) + MINIMUM_PLAYER_HP_AND_MP;
	}
};

#endif