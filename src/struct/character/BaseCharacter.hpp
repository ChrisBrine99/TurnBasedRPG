#ifndef BASE_CHARACTER_HPP
#define BASE_CHARACTER_HPP

#include "../../utility/BattleMacros.hpp"
#include "../../utility/CharacterMacros.hpp"
#include "../../utility/DataMacros.hpp"
#include "../../utility/UtilityFunctions.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <vector>

struct ActiveSkill;

typedef std::pair<uint8_t, uint8_t> Affinity;
struct BaseCharacter {
	std::array<char, CHR_EFFECTIVE_NAME_SIZE>
							name;						// Stores a 17-character string (16 empty slots and 1 null-terminator at max capacity) for the character's name.
	uint8_t					level;						// The character's current level; ranging between 1 and 100.
	std::array<uint8_t, CHR_STAT_COUNT>					// The base values for the character's seven main stats. These are the values that can be increased when a player character levels up. 
							statBase;					
	std::array<int8_t,  CHR_STAT_COUNT>					// A bonus value that is applied on top of the character's base stat value. It can increase or decrease relative to the base depending on the sum of bonuses for a given stat.
							statBonus;					
	std::array<float_t, CHR_STAT_COUNT>					// A multiplicative value that is applied to the sum of a stat's base and bonus value.
							statMultiplier;				
	uint16_t				curHitpoints;				// The character's current remaining hitpoints.
	uint16_t				curMagicpoints;				// The character's current remaining magicpoints.
	uint16_t				maxHitpointBase;			// The base value for the character's maximum hitpoints. A player character has this value set through a formula involving their level and endurance stat.
	int16_t					maxHitpointBonus;			// A bonus value that is applied on top of the character's base maximum hitpoints. It can increase or decrease the value depending on the sum of bonuses.
	float_t					maxHitpointMultiplier;		// A multiplicative value that is applied to the sum of the character's base and bonus maximum hitpoint values.
	uint16_t				maxMagicpointBase;			// The base value for the character's maximum magicpoints. A player character has this value set through a formula involving their level and magic stat.
	int16_t					maxMagicpointBonus;			// A bonus valud that is applied on top of the character's base maximum magicpoints. It can increase or decrease the value depending on the sum of bonuses.
	float_t					maxMagicpointMultiplier;	// A multiplicative value that is applied to the sum of the character' base and bonus maximum magicpoint values.
	std::vector<uint16_t>	activeSkills;				// The list of skills available to the character during battle; capped to a maximum of 6 for player characters.
	std::array<Affinity, MAIN_AFFINITY_COUNT>			// Stores two values: the affinity's unique id value, and the effect that affinity has on the character.
							resistances;				
	uint16_t				id;							// The id value for the character which should match the value found within "characters.json".
	ActiveSkill*			basicAttack;				// TODO -- replace this with an ID value eventually.

	static std::array<uint8_t, MAIN_AFFINITY_COUNT>
							resistIndex;				

public: // Constructor/Destructor Declarations
	BaseCharacter() = delete;
	BaseCharacter(uint16_t _id);
	BaseCharacter(BaseCharacter& _other) = delete;
	~BaseCharacter();

public: // Publicly Accessible Utility Function Definitions

	// Returns the character's current total for a given stat. It will clamp the value to be between 1 and 100 should the
	// calculation output a value outside of that range.
	inline uint8_t GetCurrentStatTotal(uint8_t _index) const {
		if (_index >= CHR_STAT_COUNT) // Invalid indexes are defaulted to a stat value of one.
			return CHR_MIN_STAT_VALUE;
		return std::clamp(uint8_t((statBase[_index] + statBonus[_index]) * statMultiplier[_index]), CHR_MIN_STAT_VALUE, CHR_MAX_STAT_VALUE);
	}

	// Returns the character's current maximum hitpoints; a combination of the base and bonus values added together which are 
	// then multiplied by whatever the value within "maxHitpointsMultiplier". currently is. This value is then truncated into 
	// a uint16_t before it is returned as a constant value. The default lowest value is 1 hitpoint for any character.
	inline uint16_t GetMaxHitpointsTotal() const {
		return std::max(uint16_t((maxHitpointBase + maxHitpointBonus) * maxHitpointMultiplier), CHR_MIN_HP_AND_MP);
	}

	// Returns the character's current maximum magicpoints; a combination of the base and bonus values added together which 
	// are then multiplied by whatever the value within "maxMagicpointsMultiplier". currently is. This value is then truncated 
	// into a uint16_t before it is returned as a constant value. The default lowest value is 1 magicpoint for any character.
	inline uint16_t GetMaxMagicpointsTotal() const {
		return std::max(uint16_t((maxMagicpointBase + maxMagicpointBonus) * maxMagicpointMultiplier), CHR_MIN_HP_AND_MP);
	}

	// Returns the character's resistance to a given affinity. If will first check to see if there is data found within the
	// top 4 bits of the value. If so, it will utilize that value over the character's base value. Otherwise, that original
	// value is utilized. If both bit groups have no data, the default value of EFFECT_NORMAL is returned.
	inline uint8_t GetResistance(uint8_t _affinity) const {
		for (auto& _data : resistances) {
			if (_data.first != _affinity)
				continue; // Skip each index until the desired affinity has been reached.

			uint8_t _resistance = (_data.second & OVERWRITE_RESIST_MASK) >> 4;
			if (_resistance) { return std::clamp(_resistance, EFFECT_REFLECT, EFFECT_BREAK); } 
			else { return std::clamp(uint8_t(_data.second & BASE_RESIST_MASK), EFFECT_REFLECT, EFFECT_BREAK); }
		}
		return EFFECT_NORMAL;
	}

private: // Hidden Utility Function Declarations
	void SetBasicAttackAttributes(uint16_t _id);
};

#endif