#include "Skill.hpp"

#include "../../Singletons/BattleManager.hpp"
#include "Combatant.hpp"

#include <cmath>

void Skill::UsePhysicalSkillGeneric(Combatant* _target) {
	BattleManager* _manager = GET_SINGLETON(BattleManager);
	if (!AccuracyCheck(_manager->curCombatant, _target))
		return;

	float_t _damage = PhysicalDamageCalculation(_manager->curCombatant, _target);
}

void Skill::UseMagicSkillGeneric(Combatant* _target) {
	BattleManager* _manager = GET_SINGLETON(BattleManager);
	if (!AccuracyCheck(_manager->curCombatant, _target))
		return;

	float_t _damage = MagicDamageCalculation(_manager->curCombatant, _target);
}

void Skill::UseVoidSkillGeneric(Combatant* _target) {
	BattleManager* _manager = GET_SINGLETON(BattleManager);
	if (!AccuracyCheck(_manager->curCombatant, _target))
		return;

	float_t _damage = VoidDamageCalculation(_manager->curCombatant, _target);
}

void Skill::UseMagicSkillPlusEffect(Combatant* _target) {
	BattleManager* _manager = GET_SINGLETON(BattleManager);
	if (!AccuracyCheck(_manager->curCombatant, _target))
		return;

	AdditionalEffectCheck(_target); // Attempt to apply one of the skill's possible status ailment on top of damaging the target.
	float_t _damage = MagicDamageCalculation(_manager->curCombatant, _target);
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

float_t Skill::PhysicalDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage	= float_t(basePower);
	_damage	= (_damage / 2.0f) + (_damage * float_t(_caster->stats[STAT_STRENGTH]) / 15.0f);
	_damage	= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_ENDURANCE]), 0.2f);
	return _damage;
}

float_t Skill::MagicDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage	= float_t(basePower);
	_damage	= _damage + (_damage * float_t(_caster->stats[STAT_MAGIC]) / 25.0f);
	_damage	= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_INTELLIGENCE]), 0.2f);
	return _damage;
}

float_t Skill::VoidDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage = float_t(basePower);
	_damage = _damage + (_damage * std::sqrt(float_t(_caster->stats[STAT_STRENGTH] + _caster->stats[STAT_MAGIC]) / 3.0f));
	_damage = _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_ENDURANCE] + _target->stats[STAT_INTELLIGENCE]), 0.1f);
	return _damage;
}