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

	Combatant() = delete; // Default constructor shouldn't exist. A combatant will need a character to copy data from.

	// The Combatant's constructor, which will take in a pointer to the character that they will represent within the battle.
	// It will copy all the relevant data over from that character before storing the pointer into its own "character" 
	// variable just in case it needs to be referenced during the battle. Other initial calculations are also performed
	// within the constructor as required.
	Combatant(BaseCharacter* _character) :
		level(_character->level),
		stats(),
		curHitpoints(_character->curHitpoints),
		maxHitpoints(_character->GetMaxHitpointsTotal()),
		maxHitpointMultiplier(1.0f),
		curMagicpoints(_character->curMagicpoints),
		maxMagicpoints(_character->GetMaxMagicpointsTotal()),
		maxMagicpointMultiplier(1.0f),
		activeSkills(),
		baseSpeed(0ui16),
		statModifiers(0ui16),
		flags(0ui32),
		character(_character)
	{ // Copy over the character's current stats as well as all the active skills they have available to them within a battle.
		for (size_t i = 0ui64; i < STAT_COUNT; i++)
			stats[i] = _character->GetCurrentStatTotal(uint8_t(i));

		activeSkills.reserve(_character->activeSkills.size());
		for (uint16_t _skill : _character->activeSkills)
			activeSkills.push_back(_skill);

		// On top of copying over the relevant data, the Combatant will also have its base speed calculated upon creation.
		// Each Combatant will have a minimum possible speed of 13 assuming both their level and Agility stat are a value
		// of one.
		baseSpeed = uint16_t(level) + (uint16_t(stats[STAT_AGILITY]) * 2ui16) + 10ui16;
	}

	// The Combatant should never be copied (Only ever referenced), so its copy constructor should be unavailable to utilize 
	// anywhere within the code. This is an exception compared to the standard base character, which is similar to a 
	// Combatant with a few unique exceptions.
	Combatant(Combatant& _other) = delete;
};

#endif