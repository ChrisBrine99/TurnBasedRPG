#ifndef ACTIVE_SKILL_HPP
#define ACTIVE_SKILL_HPP

#include "Skill.hpp"

class BattleScene;
struct Combatant;

struct ActiveSkill : public Skill {
	uint16_t			hpCost;			// Determines how much HP a skill will consume upon its use.
	uint16_t			mpCost;			// Determines how much MP a skill will consume upon its use.
	uint16_t			basePower;		// Value that is put into the damage formula to determine how much damage it will do to a given target.
	uint8_t				accuracy;		// The value from 0-255 will represent the accracy of the attack, which determines the percentage out of 100 to hit a target.
	uint8_t				hitCount;		// Top 4 bits hold the maximum number of hits from 0-15; bottom holds the minimum amount from 0-15.
	uint8_t				critChance;		// The percentage chance out of 255 that a skill has to inflict a critial hit on the target(s).
	uint8_t				critBonus;		// Percentage bonus to apply on top of the standard 75% bonus critical hits have to a skill's damage.
	uint8_t				recoilPower;	// The amount of hp the caster will lost when using this skill (The value is between 0 and 255; 255 being the target's total maximum hp).
	std::array<uint8_t, ASKILL_MAX_UNIQUE_EFFECTS>
						addedEffects;	// Stores up to four 8-bit integers that are equal to various status ailments that a skill can apply onto a target when used.
	uint8_t				effectChance;	// Stores a value between 0 and 255 that simply determines the percentage chance of an added effect occurring.
	uint16_t			buffAmount;		// Much like how buffs are stored within the Combatant struct, this stores how much to debuff/buff a character by when hit by the skill.
	//	NOTE --- The final bit in "buffAmount" will toggle an override that will completely replace the target's current buff value with the skill's.
	uint8_t				buffDuration;	// Determines the number of turns that the buff/debuff will be active for on the target(s).
	uint8_t				dmgMultiplier;	// A multiplicative value between 0 and 255 that determines how much of a percentage relative to the skill's calculated damage to apply as a bonus. Maxes out at a 2x bonus to damage.
	uint16_t			healPower;		// Similar to "basePower" but will exclusively be used to heal the target(s) when using the skill (Or caster in certain contexts).

	// The function that will be called upon the skill's use in battle.
	void (ActiveSkill::* useFunction)(BattleScene*, Combatant*);

public: // Constructor/Destructor Declarations
	ActiveSkill();
	ActiveSkill(const ActiveSkill& _other) = delete;
	~ActiveSkill() = default;

public: // Skill Use Function Declarations
	void ExecuteUseFunction(BattleScene* _scene, Combatant* _target);

	// --- General Active Skill Use Functions --- //
	void UsePhysicalSkillGeneric(BattleScene* _scene, Combatant* _target);
	void UseMagicSkillGeneric(BattleScene* _scene, Combatant* _target);
	void UseVoidSkillGeneric(BattleScene* _scene, Combatant* _target);
	void UseHealingSkillGeneric(BattleScene* _scene, Combatant* _target);

	// --- Special Case Magic Skill Use Functions --- //
	void UseMagicSkillPlusEffect(BattleScene* _scene, Combatant* _target);

private: // Skill Utility Function Declarations
	bool AccuracyCheck(BattleScene* _scene, Combatant* _target) const;
	bool CriticalCheck(BattleScene* _scene, Combatant* _target) const;
	void AdditionalEffectCheck(Combatant* _target);

	void PhysicalDamageCalculation(BattleScene* _scene, Combatant* _target);
	void MagicDamageCalculation(BattleScene* _scene, Combatant* _target);
	void VoidDamageCalculation(BattleScene* _scene, Combatant* _target);

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