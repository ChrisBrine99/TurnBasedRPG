#ifndef PASSIVE_SKILL_HPP
#define PASSIVE_SKILL_HPP

#include "Skill.hpp"

struct PassiveSkill : public Skill {
	int16_t				maxHpBonus;			// Determines how much to add or remove from the character's maximum HP.
	int16_t				maxMpBonus;			// Determines how much to add or remove from the character's maximum MP.
	uint8_t				maxHpMultiplier;	// A multiplicative value (value/255) against the character's maximum HP; ranging from no change to a 2x multiplier.
	uint8_t				maxMpMultiplier;	// A multiplicative value (value/255) against the character's maximum MP; ranging from no change to a 2x multiplier.
	uint16_t			affinityFlags;		// Affinities that will have their damage increased by the skill's affinityModifier variable.
	uint8_t				affinityModifier;	// A multiplicative value (value/255) against the damage of a skill from a valid affinity (Its value is 1 within the affinityFlags variable); ranging from no change to a 2x multiplier.
	uint8_t				buffDurationBonus;	// Increases the duration of the standard buffing/debuffing skills.
	uint8_t				critChanceBonus;	// A multiplicative value (value/255) against a skill's base crit chance; ranging from no change to a 2x multiplier.
	uint8_t				critDamageBonus;	// A multiplicative value (value/255) against a critical hit's damage; ranging from no change to a 2x multiplier.
	uint8_t				resistOverwrite;	// The value to assign to a character's resistance. Resistance flags (Stored within the top half of "flags") that are set to 1 will have their base values overwritten by this one.
	uint16_t			useSkillID;			// The ID for the skill this passive will use whenever it is activated in battle.

public: // Constructor/Destructor Declarations
	PassiveSkill();
	PassiveSkill(const PassiveSkill& _other) = delete;
	~PassiveSkill() = default;
};

#endif