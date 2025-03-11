#ifndef SKILL_HPP
#define SKILL_HPP

#include "../../Utils/BattleMacros.hpp"
#include "../../Utils/DataMacros.hpp"
#include "../../Utils/GeneralMacros.hpp"

#include <cinttypes>
#include <string>

struct Skill {
	std::string		name;				// The name of the skill.
	std::string		description;		// Provides generic details about what the skill does within the game.
	uint16_t		id;					// Unique identifier between 0 and 65,535 that can be used to reference the skill's data outside this struct.
	uint16_t		basePower;			// Value that is put into the damage formula to determine how much damage it will do to a given target.
	uint8_t			baseCost;			// Determines how much MP a skill will consume upon its use.
	uint8_t			affinity;			// Unique identifier that determines what the skill will do when used against an enemy or on an ally.
	uint8_t			accuracy;			// The value from 0-255 will represent the accracy of the attack, which determines the percentage out of 100 to hit a target.
	uint8_t			hitAmount;			// Top 4 bits hold the maximum number of hits from 0-15; bottom holds the minimum amount from 0-15.
	void			(*useFunction)();	// The function that will be called upon the skill's use.

	// A skill's default constructor; initializes every one of its variables with default values.
	Skill() :
		name("Unknown"),
		description("N/A"),
		id(ID_INVALID),
		basePower(0ui16),
		baseCost(0ui8),
		affinity(AFFINITY_INVALID),
		accuracy(0ui8),
		hitAmount(0ui8),
		useFunction(nullptr)
	{}

	// A skill struct should never be copied (All currently loaded skills will exist within a master list that characters 
	// reference whenever they required information about the Skill to be utilized. So, the default copy contstructor is
	// deleted so it doesn't exist.
	Skill(Skill& _other) = delete;
};

#endif