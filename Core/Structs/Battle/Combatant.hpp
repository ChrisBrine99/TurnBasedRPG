#ifndef COMBATANT_HPP
#define COMBATANT_HPP

#include "../Characters/BaseCharacter.hpp"

struct Combatant {
	uint8_t							level;
	std::array<uint8_t, STAT_COUNT> stats;

	uint16_t						curHitpoints;
	uint16_t						maxHitpoints;
	float_t							maxHitpointMultiplier;

	uint16_t						curMagicpoints;
	uint16_t						maxMagicpoints;
	float_t							maxMagicpointMultiplier;

	std::vector<uint16_t>			activeSkills;

	uint16_t						baseSpeed;
	uint16_t						statModifiers;
	uint32_t						flags;

	BaseCharacter*					character;

	// Default constructor; initializes all variables to their default values. Upon activation, these values will all be 
	// replaced by the equivalent values found within the data copied over from the character this Combatant will represent
	// within the battle.
	Combatant() :
		level(1ui8),
		stats(std::array<uint8_t, STAT_COUNT>()),
		curHitpoints(0ui16),
		maxHitpoints(0ui16),
		maxHitpointMultiplier(1.0f),
		curMagicpoints(0ui16),
		maxMagicpoints(0ui16),
		maxMagicpointMultiplier(1.0f),
		activeSkills(std::vector<uint16_t>()),
		baseSpeed(0ui16),
		statModifiers(0ui16),
		flags(0u),
		character(nullptr)
	{ 
		stats.fill(1ui8); // Populate the array with default values of 1 for each stat.
		activeSkills.reserve(PLAYER_SKILL_LIMIT); // Reserve at least enough memory to store the maximum number of skills a player character can use in battle.
	}

	// Copy over the required data from the desired character instance. Then, apply any additional flags on top of the flag 
	// that signals to the BattleManager that the combatant is now active in the battle so it can be added to the turn order.
	inline void ActivateCombatant(BaseCharacter* _character, uint32_t _flags) {
		level = _character->level; // Make sure the level is copied over.

		for (size_t i = 0ui64; i < STAT_COUNT; i++) // Copy over the seven main stat values.
			stats[i] = _character->GetCurrentStatTotal(uint8_t(i));

		// Get the character's current and maximum hitpoints.
		curHitpoints = _character->curHitpoints;
		maxHitpoints = _character->GetMaxHitpointsTotal();
		maxHitpointMultiplier = 1.0f; // Reset back to its default

		// Get the character's current and maximum magicpoints.
		curMagicpoints = _character->curMagicpoints;
		maxMagicpoints = _character->GetMaxMagicpointsTotal();
		maxMagicpointMultiplier = 1.0f; // Reset back to its default

		// Since the vector doesn't need to be copied, it can simply be referenced for the battle.
		activeSkills = _character->activeSkills;

		// Calculate/recalculate speed based on the character's level and agility stats.
		baseSpeed = uint16_t(level) + (uint16_t(stats[STAT_AGILITY]) * 2ui16) + 10ui16;

		// Stores the bits 0011 0110 1101 1011 which equates to each modifier being set to +3 which is actually
		// considered 0 by the BattleManager since it offsets the values by -3 during calculations.
		statModifiers = 0x36DBui16;

		// Finally, set the flags for the combatant and assign its character to the stored pointer so it can be referenced 
		// whenever required during the course of a battle. The active flag is always set upon calling this function.
		flags = _flags | FLAG_COMBATANT_ACTIVE;
		character = _character;
	}

	// Isolates the bits within the "statModifiers" variable that represent the current modifier values for each of the 
	// Combatant's battle stats. In this state they range between 0 and 7, but they will be offset by -3 to actually be
	// within the range of -3 to +4, which is what is required by the BattleManager in order to execute the proper formulas 
	// for each respective battle stat should they need to affect that particular aspect of the character's performance in
	// the current battle.
	inline int8_t GetCurrentStatModifier(uint16_t _modifierBitMask) const {
		switch (_modifierBitMask) {
		case ATTACK_MODIFIER:		return int8_t( statModifiers & ATTACK_MODIFIER );
		case DEFENCE_MODIFIER:		return int8_t((statModifiers & DEFENCE_MODIFIER)	>> 3);
		case ACCURACY_MODIFIER:		return int8_t((statModifiers & ACCURACY_MODIFIER)	>> 6);
		case EVASION_MODIFIER:		return int8_t((statModifiers & EVASION_MODIFIER)	>> 9);
		case SPEED_MODIFIER:		return int8_t((statModifiers & SPEED_MODIFIER)		>> 12);
		default:					return 0i8;
		}
	}
};

#endif