#ifndef BASE_CHARACTER_HPP
#define BASE_CHARACTER_HPP

#include "../../Utils/CharacterMacros.hpp"
#include "../../Utils/DataMacros.hpp"
#include "../../Utils/UtilityFunctions.hpp"

#include <string>
#include <array>
#include <vector>

struct BaseCharacter {
	std::string						name;
	uint8_t							level;

	std::array<uint8_t, STAT_COUNT> statBase;
	std::array<int8_t,  STAT_COUNT>	statBonus;
	std::array<float_t, STAT_COUNT> statMultiplier;

	uint16_t						curHitpoints;
	uint16_t						curMagicpoints;

	uint16_t						maxHitpointBase;
	int16_t							maxHitpointBonus;
	float_t							maxHitpointMultiplier;

	uint16_t						maxMagicpointBase;
	int16_t							maxMagicpointBonus;
	float_t							maxMagicpointMultiplier;
	
	std::vector<uint16_t>			activeSkills;

	// Default constructor that will initialize all variables/data structures with their default values. On top of that, it 
	// will reserve enough memory to store 6 elements within the vector; due to that being the amount that will always be
	// utilized on average since it's the limit to the amount a player character can take into battle, but enemies are immune
	// to this limit.
	BaseCharacter() :
		level(0ui8),
		statBase({ 1ui8, 1ui8, 1ui8, 1ui8, 1ui8, 1ui8, 1ui8 }),
		statBonus({ 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8 }),
		statMultiplier({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }),
		curHitpoints(0ui16),
		curMagicpoints(0ui16),
		maxHitpointBase(0ui16),
		maxHitpointBonus(0i16),
		maxHitpointMultiplier(1.0f),
		maxMagicpointBase(0ui16),
		maxMagicpointBonus(0i16),
		maxMagicpointMultiplier(1.0f),
		activeSkills()
	{ // Reserve a small chunk of memory for the character's pool of usable skills.
		activeSkills.reserve(PLAYER_SKILL_LIMIT);
	}

	// Ensures that using the copy constructor isn't possible anywhere within the code, as characters should only ever be 
	// referenced and never copied.
	BaseCharacter(BaseCharacter& _other) = delete;

	// Default destructor that will simply clear out the vector and reduce its capacity back down to 0.
	~BaseCharacter() {
		activeSkills.clear();
		activeSkills.shrink_to_fit();
	}

	// Returns the character's current total for a given stat. It will clamp the value to be between 1 and 100 should the
	// calculation output a value outside of that range.
	inline uint8_t GetCurrentStatTotal(uint8_t _index) const {
		if (_index >= STAT_COUNT) // Invalid indexes are defaulted to a stat value of one.
			return MINIMUM_STAT_VALUE;
		return ValueClamp((uint8_t)((statBase[_index] + statBonus[_index]) * statMultiplier[_index]), MINIMUM_STAT_VALUE, MAXIMUM_STAT_VALUE);
	}

	// Returns the character's current maximum hitpoints; a combination of the base and bonus values added together which are 
	// then multiplied by whatever the value within "maxHitpointsMultiplier". currently is. This value is then truncated into 
	// a uint16_t before it is returned as a constant value. The default lowest value is 1 hitpoint for any character.
	inline uint16_t GetMaxHitpointsTotal() const {
		return ValueLowerLimit((uint16_t)((maxHitpointBase + maxHitpointBonus) * maxHitpointMultiplier), MINIMUM_HP_AND_MP);
	}

	// Returns the character's current maximum magicpoints; a combination of the base and bonus values added together which 
	// are then multiplied by whatever the value within "maxMagicpointsMultiplier". currently is. This value is then truncated 
	// into a uint16_t before it is returned as a constant value. The default lowest value is 1 magicpoint for any character.
	inline uint16_t GetMaxMagicpointsTotal() const {
		return ValueLowerLimit((uint16_t)((maxMagicpointBase + maxMagicpointBonus) * maxMagicpointMultiplier), MINIMUM_HP_AND_MP);
	}
};

#endif