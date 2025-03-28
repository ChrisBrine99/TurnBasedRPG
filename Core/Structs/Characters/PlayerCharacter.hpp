#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

#include "BaseCharacter.hpp"

struct PlayerCharacter : public BaseCharacter {
	uint32_t						curExperience;
	uint32_t						nextLevelExperience;

	std::vector<uint16_t>			knownSkills;

public: // Constructor/Destructor Definitions
	PlayerCharacter() : 
		BaseCharacter(),
		curExperience(0ui32),
		nextLevelExperience(0Ui32),
		knownSkills()
	{}
	PlayerCharacter(const PlayerCharacter& _other) = delete;
	~PlayerCharacter() = default;

public: // Publicly Accessible Utilty Function Definitions

	// Adds the given amount to the player character's current experience value. Then it checks to see if that value exceeds
	// the current requirement to reach the next level. If so, a loop will begin that increases their level until the 
	// calculated requirement exceeds the current experience value OR their level reaches 100. The character's max HP and MP 
	// values are updated to match the new level value after the loop has completed execution.
	inline void RewardExperience(uint32_t _amount) {
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