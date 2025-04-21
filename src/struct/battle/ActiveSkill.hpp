#ifndef ACTIVE_SKILL_HPP
#define ACTIVE_SKILL_HPP

#include "Skill.hpp"

struct ActiveSkill : public Skill {
	uint16_t			hpCost;			// Determines how much HP a skill will consume upon its use.
	uint16_t			mpCost;			// Determines how much MP a skill will consume upon its use.
	uint16_t			basePower;		// Value that is put into the damage formula to determine how much damage it will do to a given target.
	uint8_t				accuracy;		// The value from 0-255 will represent the accracy of the attack, which determines the percentage out of 100 to hit a target.
	uint8_t				hitCount;		// Top 4 bits hold the maximum number of hits from 0-15; bottom holds the minimum amount from 0-15.
	uint8_t				critChance;		// The percentage chance out of 255 that a skill has to inflict a critial hit on the target(s).
	uint8_t				critBonus;		// Percentage bonus to apply on top of the standard 75% bonus critical hits have to a skill's damage.
	uint8_t				recoilPower;	// The amount of hp the caster will lost when using this skill (The value is between 0 and 255; 255 being the target's total maximum hp).
	std::array<uint8_t, SKILL_MAX_UNIQUE_EFFECTS>
		addedEffects;	// Stores up to four 8-bit integers that are equal to various status ailments that a skill can apply onto a target when used.
	uint8_t				effectChance;	// Stores a value between 0 and 255 that simply determines the percentage chance of an added effect occurring.
	uint16_t			buffAmount;		// Much like how buffs are stored within the Combatant struct, this stores how much to debuff/buff a character by when hit by the skill.
	//	NOTE --- The final bit in "buffAmount" will toggle an override that will completely replace the target's current buff value with the skill's.
	uint8_t				buffDuration;	// Determines the number of turns that the buff/debuff will be active for on the target(s).
	uint8_t				dmgMultiplier;	// A multiplicative value between 0 and 255 that determines how much of a percentage relative to the skill's calculated damage to apply as a bonus. Maxes out at a 2x bonus to damage.
	uint16_t			healPower;		// Similar to "basePower" but will exclusively be used to heal the target(s) when using the skill (Or caster in certain contexts).

public: // Constructor/Destructor Declarations
	ActiveSkill();
	ActiveSkill(const ActiveSkill& _other) = delete;
	~ActiveSkill() = default;
};

#endif