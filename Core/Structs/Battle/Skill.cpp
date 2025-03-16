#include "Skill.hpp"

#include "../../Singletons/BattleManager.hpp"
#include "Combatant.hpp"

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

	float_t _damage = SpecialDamageCalculation(_manager->curCombatant, _target);
}

void Skill::UseMagicSkillPlusEffect(Combatant* _target) {

}

bool Skill::AccuracyCheck(Combatant* _caster, Combatant* _target) const {
	uint16_t _accuracyBuff	= uint16_t(_caster->GetCurrentStatModifier(ACCURACY_MODIFIER) - _target->GetCurrentStatModifier(EVASION_MODIFIER));
	uint16_t _accuracy		= accuracy;

	if (_accuracyBuff > 0ui16)		{ _accuracy *= (9ui16 + _accuracyBuff) / 9ui16; }
	else if (_accuracyBuff < 0ui16) { _accuracy *= 9ui16 / (9ui16 - _accuracyBuff); }

	return (_accuracy >= uint16_t(std::rand() % 0x00FFui16));
}

float_t Skill::PhysicalDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage	= float_t(basePower);
	_damage	= (_damage / 2.0f) + (_damage * _caster->stats[STAT_STRENGTH] / 15.0f);
	_damage	= DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_ENDURANCE]), 0.2f);
	return _damage;
}

float_t Skill::MagicDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage	= float_t(basePower);
	_damage	= _damage + (_damage * _caster->stats[STAT_MAGIC] / 25.0f);
	_damage	= DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_INTELLIGENCE]), 0.2f);
	return _damage;
}

float_t Skill::SpecialDamageCalculation(Combatant* _caster, Combatant* _target) {
	float_t _damage = float_t(basePower);
	_damage = _damage + (_damage * std::sqrt(float_t(_caster->stats[STAT_STRENGTH] + _caster->stats[STAT_MAGIC]) / 3.0f));
	_damage = DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage = _damage / std::pow(float_t(_target->stats[STAT_ENDURANCE] + _target->stats[STAT_INTELLIGENCE]), 0.1f);
	return _damage;
}