#ifndef SKILL_HPP
#define SKILL_HPP

#include "../../Utils/BattleMacros.hpp"
#include "../../Utils/DataMacros.hpp"
#include "../../Utils/GeneralMacros.hpp"

#include <cinttypes>
#include <string>
#include <array>

struct Combatant;

struct Skill {
	std::string					name;			// The name of the skill.
	std::string					description;	// Provides generic details about what the skill does within the game.
	uint16_t					id;				// Unique identifier between 0 and 65,535 that can be used to reference the skill's data outside this struct.
	uint8_t						affinity;		// Unique identifier that determines what the skill will do when used against an enemy or on an ally.
	uint8_t						targeting;		// Determines how the skill will affect combatants on the casters side of the field and on the opposite side.
	uint8_t						hpCost;			// Determines how much HP a skill will consume upon its use.
	uint8_t						mpCost;			// Determines how much MP a skill will consume upon its use.
	uint16_t					basePower;		// Value that is put into the damage formula to determine how much damage it will do to a given target.
	uint8_t						accuracy;		// The value from 0-255 will represent the accracy of the attack, which determines the percentage out of 100 to hit a target.
	uint8_t						hitCount;		// Top 4 bits hold the maximum number of hits from 0-15; bottom holds the minimum amount from 0-15.
	std::array<uint8_t, SKILL_MAX_UNIQUE_EFFECTS>
								addedEffects;	// Stores up to four 8-bit integers that are equal to various status ailments that a skill can apply onto a target when used.
	std::array<uint8_t, SKILL_MAX_UNIQUE_EFFECTS>	
								effectChance;	// Stores another group of four 8-bit integers that simply determine the chance of inflicting each added effect.

	// The function that will be called upon the skill's use in battle.
	void (Skill::*useFunction)(Combatant*);

	// A skill's default constructor; initializes every one of its variables with default values.
	Skill() :
		name("Unknown"),
		description("N/A"),
		id(ID_INVALID),
		basePower(0ui16),
		affinity(AFFINITY_INVALID),
		accuracy(0ui8),
		hpCost(0ui8),
		mpCost(0ui8),
		hitCount(0ui8),
		targeting(TARGET_INVALID),
		addedEffects(),
		effectChance(),
		useFunction(nullptr)
	{ // Simply populates both arrays with default values of 255 and 0, respectively.
		addedEffects.fill(AILMENT_INVALID);
		effectChance.fill(0ui8);
	}

	// The function that is responsible for executing the Skill's "use" function. If the value in "useFunction" is nullptr, 
	// this function will do nothing and the skill will do nothing within the battle.
	inline void ExecuteUseFunction(Combatant* _target) {
		if (useFunction == nullptr)
			return;
		((*this).*(this->useFunction))(_target);
	}

public: // Skill Use Function Declarations (Defined within Skill.cpp)
	void UsePhysicalSkillGeneric(Combatant* _target);
	void UseMagicSkillGeneric(Combatant* _target);
	void UseVoidSkillGeneric(Combatant* _target);

	void UseMagicSkillPlusEffect(Combatant* _target);

private: // Skill Utility Function Declarations (Defined within Skill.cpp)
	bool AccuracyCheck(Combatant* _caster, Combatant* _target) const;
	void AdditionalEffectCheck(Combatant* _target);

	float_t PhysicalDamageCalculation(Combatant* _caster, Combatant* _target);
	float_t MagicDamageCalculation(Combatant* _caster, Combatant* _target);
	float_t VoidDamageCalculation(Combatant* _caster, Combatant* _target);

	// Returns either a value calculated through using the buff formula (The value passed in as the argument was above 0) or 
	// the debuff formula (The value passed in was below 0).
	inline float_t DamageBuffCalculation(int8_t _buffValue) {
		if (_buffValue > 0ui8)		{ return (4.0f + float_t(_buffValue)) / 4.0f; }
		else if (_buffValue < 0ui8)	{ return (4.0f / 4.0f - float_t(_buffValue)); }
		return 1.0f; // No multiplicative adjustment is required; return a floating point one.
	}
};

#endif