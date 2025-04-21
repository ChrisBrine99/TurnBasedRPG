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
	std::string	name;
	uint8_t		level;

	std::array<uint8_t, STAT_COUNT> statBase;
	std::array<int8_t,  STAT_COUNT>	statBonus;
	std::array<float_t, STAT_COUNT> statMultiplier;

	uint16_t curHitpoints;
	uint16_t curMagicpoints;

	uint16_t maxHitpointBase;
	int16_t	 maxHitpointBonus;
	float_t	 maxHitpointMultiplier;

	uint16_t maxMagicpointBase;
	int16_t	 maxMagicpointBonus;
	float_t	 maxMagicpointMultiplier;
	
	std::vector<uint16_t> activeSkills;

	std::array<Affinity, MAIN_AFFINITY_COUNT>		resistances;
	static std::array<uint8_t, MAIN_AFFINITY_COUNT> resistIndex;

	uint16_t		id;
	ActiveSkill*	basicAttack;

public: // Constructor/Destructor Declarations
	BaseCharacter() = delete;
	BaseCharacter(uint16_t _id);
	BaseCharacter(BaseCharacter& _other) = delete;
	~BaseCharacter();

public: // Publicly Accessible Utility Function Definitions

	// Returns the character's current total for a given stat. It will clamp the value to be between 1 and 100 should the
	// calculation output a value outside of that range.
	inline uint8_t GetCurrentStatTotal(uint8_t _index) const {
		if (_index >= STAT_COUNT) // Invalid indexes are defaulted to a stat value of one.
			return MINIMUM_STAT_VALUE;
		return std::clamp(uint8_t((statBase[_index] + statBonus[_index]) * statMultiplier[_index]), MINIMUM_STAT_VALUE, MAXIMUM_STAT_VALUE);
	}

	// Returns the character's current maximum hitpoints; a combination of the base and bonus values added together which are 
	// then multiplied by whatever the value within "maxHitpointsMultiplier". currently is. This value is then truncated into 
	// a uint16_t before it is returned as a constant value. The default lowest value is 1 hitpoint for any character.
	inline uint16_t GetMaxHitpointsTotal() const {
		return std::max(uint16_t((maxHitpointBase + maxHitpointBonus) * maxHitpointMultiplier), MINIMUM_HP_AND_MP);
	}

	// Returns the character's current maximum magicpoints; a combination of the base and bonus values added together which 
	// are then multiplied by whatever the value within "maxMagicpointsMultiplier". currently is. This value is then truncated 
	// into a uint16_t before it is returned as a constant value. The default lowest value is 1 magicpoint for any character.
	inline uint16_t GetMaxMagicpointsTotal() const {
		return std::max(uint16_t((maxMagicpointBase + maxMagicpointBonus) * maxMagicpointMultiplier), MINIMUM_HP_AND_MP);
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