#ifndef SKILL_HPP
#define SKILL_HPP

#include "../../utility/BattleMacros.hpp"
#include "../../utility/DataMacros.hpp"
#include "../../utility/GeneralMacros.hpp"

#include <string>
#include <array>

class BattleScene;
struct Combatant;

struct Skill {
	std::string			name;			// The name of the skill.
	uint16_t			id;				// Unique identifier between 0 and 65,535 that can be used to reference the skill's data outside this struct.
	uint8_t				affinity;		// Unique identifier that determines what the skill will do when used against an enemy or on an ally.
	uint8_t				targeting;		// Determines how the skill will affect combatants on the casters side of the field and on the opposite side.
	uint32_t			flags;			// Additional paramaters that can be toggled on or off to enable/disable certain properties for the skill.

	// The function that will be called upon the skill's use in battle.
	void (Skill::*useFunction)(BattleScene*, Combatant*, Skill*);

public: // Constructor/Destructor Declarations
	Skill();
	Skill(const Skill& _other) = delete;
	~Skill() = default;

public: // Skill Use Function Declarations
	void ExecuteUseFunction(BattleScene* _scene, Combatant* _target, Skill* _skill);

	// --- General Active Skill Use Functions --- //
	void UsePhysicalSkillGeneric(BattleScene* _scene, Combatant* _target, Skill* _skill);
	void UseMagicSkillGeneric(BattleScene* _scene, Combatant* _target, Skill* _skill);
	void UseVoidSkillGeneric(BattleScene* _scene, Combatant* _target, Skill* _skill);
	void UseHealingSkillGeneric(BattleScene* _scene, Combatant* _target, Skill* _skill);

	// --- Special Case Magic Skill Use Functions --- //
	void UseMagicSkillPlusEffect(BattleScene* _scene, Combatant* _target, Skill* _skill);

private: // Skill Utility Function Declarations
	bool AccuracyCheck(BattleScene* _scene, Combatant* _target, int8_t _baseAccuracy) const;
	bool CriticalCheck(BattleScene* _scene, Combatant* _target) const;
	void AdditionalEffectCheck(Combatant* _target);

	void PhysicalDamageCalculation(BattleScene* _scene, Combatant* _target, int16_t _basePower);
	void MagicDamageCalculation(BattleScene* _scene, Combatant* _target, int16_t _basePower);
	void VoidDamageCalculation(BattleScene* _scene, Combatant* _target, int16_t _basePower);

	void ResistanceEffect(BattleScene* _scene, float_t _damage, Combatant* _target) const;
	void HealingEffect(BattleScene* _scene, float_t _healAmount, Combatant* _target) const;

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