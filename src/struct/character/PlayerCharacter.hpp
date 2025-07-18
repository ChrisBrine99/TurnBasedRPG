#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

#include "BaseCharacter.hpp"

struct PlayerCharacter : public BaseCharacter {
	uint32_t				curExperience;			// Stores the sum of all experience earned by the character.
	uint32_t				nextLevelExperience;	// Stores the value that the player character's experience needs to exceeds before they can advance to the next level.
	std::vector<uint16_t>	knownSkills;			// A complete list of IDs for every skill the character has unlocked and available to place into their active skill list.
	std::array<uint16_t, PCHR_TOTAL_EQUIP_SLOTS>	// Stores the item IDs for the equipment the character currently has equipped. The slots are: head, body, legs, weapon, and accessory.
							equipment;				

public: // Constructor/Destructor Declarations
	PlayerCharacter() = delete;
	PlayerCharacter(uint16_t _id);
	PlayerCharacter(const PlayerCharacter& _other) = delete;
	~PlayerCharacter();

public: // Publicly Accessible Utilty Function Definitions
	void RewardExperience(uint32_t _amount);

	// Returns a value for the experience required in order for the player character to increase their current level by one. 
	// Exceptions to the formula used are when the level value is 0, 1, or any value above 100--where a 0 is returned instead.
	inline uint32_t DetermineRequiredExperience(uint8_t _level) {
		if (_level <= 1Ui8 || _level > CHR_MAX_LEVEL)
			return 0U;
		return uint32_t(1.2f * float_t(_level * _level * _level));
	}

	// Contains the formula that determines a playable character's maximum HP value, which is affected by both the level and
	// current endurance stat.
	inline uint16_t CalculateMaxBaseHitpoints() {
		return uint16_t(level * 3ui16) + uint16_t(GetCurrentStatTotal(CHR_STAT_ENDURANCE) * 4ui16) + PCHR_MIN_HP_AND_MP;
	}

	// Contains the formula that determines a playable character's maximum MP value, which is affected by both the level and
	// current endurance stat.
	inline uint16_t CalculateMaxBaseMagicpoints() {
		return uint16_t(level * 3ui16) + uint16_t(GetCurrentStatTotal(CHR_STAT_MAGIC) * 4ui16) + PCHR_MIN_HP_AND_MP;
	}
};

#endif