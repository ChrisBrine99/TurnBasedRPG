#ifndef SKILL_HPP
#define SKILL_HPP

#include "../../utility/BattleMacros.hpp"
#include "../../utility/DataMacros.hpp"
#include "../../utility/GeneralMacros.hpp"

#include <cinttypes>
#include <string>
#include <array>

class BattleScene;
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
	void (Skill::*useFunction)(BattleScene*, Combatant*);

public: // Constructor/Destructor Declarations
	Skill();
	Skill(const Skill& _other) = delete;
	~Skill() = default;

public: // Skill Use Function Declarations
	void UsePhysicalSkillGeneric(BattleScene* _scene, Combatant* _target);
	void UseMagicSkillGeneric(BattleScene* _scene, Combatant* _target);
	void UseVoidSkillGeneric(BattleScene* _scene, Combatant* _target);

	void UseMagicSkillPlusEffect(BattleScene* _scene, Combatant* _target);

	// A simple function that is responsible for calling the skill's use function through the pointer stored to the function 
	// it requires. Does nothing if "useFunction" isn't set to a proper funciton pointer.
	inline void ExecuteUseFunction(BattleScene* _scene, Combatant* _target) {
		if (useFunction == nullptr)
			return;
		((*this).*(this->useFunction))(_scene, _target);
	}

private: // Skill Utility Function Declarations (Defined within Skill.cpp)
	bool AccuracyCheck(Combatant* _caster, Combatant* _target) const;
	void AdditionalEffectCheck(Combatant* _target);

	uint16_t PhysicalDamageCalculation(Combatant* _caster, Combatant* _target);
	uint16_t MagicDamageCalculation(Combatant* _caster, Combatant* _target);
	uint16_t VoidDamageCalculation(Combatant* _caster, Combatant* _target);

	// Returns either a value calculated through using the buff formula (The value passed in as the argument was above 0) or 
	// the debuff formula (The value passed in was below 0).
	inline float_t DamageBuffCalculation(int8_t _buffValue) {
		if (_buffValue > 0ui8)		{ return (4.0f + float_t(_buffValue)) / 4.0f; }
		else if (_buffValue < 0ui8)	{ return (4.0f / 4.0f - float_t(_buffValue)); }
		return 1.0f; // No multiplicative adjustment is required; return a floating point one.
	}

	// Returns a value that is between an arbitrary range that defaults to 95% ro 105% of the input value.
	inline float_t DamageRandomize(float_t _damage, float_t _high = 1.05f, float_t _low = 0.95f) {
		return _damage * (_low + float_t(std::rand()) / float_t(RAND_MAX / (_high - _low)));
	}
};

#endif