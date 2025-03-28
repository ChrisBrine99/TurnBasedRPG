#include "Skill.hpp"

#include "../../scene/BattleScene.hpp"
#include "Combatant.hpp"

#include <cmath>
#include <iostream>

Skill::Skill() :
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

void Skill::UsePhysicalSkillGeneric(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene->curCombatant, _target))
		return;

	uint16_t _damage = PhysicalDamageCalculation(_scene->curCombatant, _target);
}

void Skill::UseMagicSkillGeneric(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene->curCombatant, _target))
		return;

	uint16_t _damage = MagicDamageCalculation(_scene->curCombatant, _target);
	_scene->UpdateHitpoints(_target, _damage);
}

void Skill::UseVoidSkillGeneric(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene->curCombatant, _target))
		return;

	uint16_t _damage = VoidDamageCalculation(_scene->curCombatant, _target);
	_scene->UpdateHitpoints(_target, _damage);
}

void Skill::UseMagicSkillPlusEffect(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene->curCombatant, _target)) {
		std::cout << "Attack Missed!" << std::endl;
		return;
	}

	AdditionalEffectCheck(_target); // Attempt to apply one of the skill's possible status ailment on top of damaging the target.
	uint16_t _damage = MagicDamageCalculation(_scene->curCombatant, _target);
	_scene->UpdateHitpoints(_target, _damage);
}

bool Skill::AccuracyCheck(Combatant* _caster, Combatant* _target) const {
	uint16_t _accuracyBuff	= uint16_t(_caster->GetCurrentStatModifier(ACCURACY_MODIFIER) - _target->GetCurrentStatModifier(EVASION_MODIFIER));
	uint16_t _accuracy		= accuracy;

	if (_accuracyBuff > 0ui16)		{ _accuracy *= (9ui16 + _accuracyBuff) / 9ui16; }
	else if (_accuracyBuff < 0ui16) { _accuracy *= 9ui16 / (9ui16 - _accuracyBuff); }

	return (_accuracy >= uint16_t(std::rand() % 0x00FFui16));
}

void Skill::AdditionalEffectCheck(Combatant* _target) {
	for (size_t i = 0ui64; i < SKILL_MAX_UNIQUE_EFFECTS; i++) {
		if (addedEffects[i] == AILMENT_INVALID)
			continue;

		// The earlier effects in the list will always have higher priority than the effects at the end. However, the later effects
		// will always be considered if the higher priority effects fail to successfully affect the target(s).
		if (effectChance[i] >= uint8_t(std::rand() % 0xFFui8) && _target->InflictStatusAilment(addedEffects[i]))
			return;
	}
}

uint16_t Skill::PhysicalDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage	= float_t(basePower);
	_damage	= (_damage / 2.0f) + (_damage * float_t(_caster->stats[STAT_STRENGTH]) / 15.0f);
	_damage	= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_ENDURANCE]), 0.2f);
	return int16_t(DamageRandomize(_damage)); // Randomly select a value between 95% and 105% of the calculated damage.
}

uint16_t Skill::MagicDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage	= float_t(basePower);
	_damage	= _damage + (_damage * float_t(_caster->stats[STAT_MAGIC]) / 25.0f);
	_damage	= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_INTELLIGENCE]), 0.2f);
	return int16_t(DamageRandomize(_damage)); // Randomly select a value between 95% and 105% of the calculated damage.
}

uint16_t Skill::VoidDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage = float_t(basePower);
	_damage = _damage + (_damage * std::sqrt(float_t(_caster->stats[STAT_STRENGTH] + _caster->stats[STAT_MAGIC]) / 3.0f));
	_damage = _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_ENDURANCE] + _target->stats[STAT_INTELLIGENCE]), 0.1f);
	return int16_t(DamageRandomize(_damage)); // Randomly select a value between 95% and 105% of the calculated damage.
}