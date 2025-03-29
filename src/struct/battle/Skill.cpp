#include "Skill.hpp"

#include "../../scene/BattleScene.hpp"
#include "../../user_interface/battle/BattleUI.hpp"
#include "../../user_interface/battle/BattleUIElement.hpp"
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
	if (!AccuracyCheck(_scene, _target))
		return;
	PhysicalDamageCalculation(_scene, _target);
}

void Skill::UseMagicSkillGeneric(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene, _target))
		return;
	MagicDamageCalculation(_scene, _target);
}

void Skill::UseVoidSkillGeneric(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene, _target))
		return;
	VoidDamageCalculation(_scene, _target);
}

void Skill::UseMagicSkillPlusEffect(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene, _target))
		return;
	AdditionalEffectCheck(_target); // Attempt to apply one of the skill's possible status ailment on top of damaging the target.
	MagicDamageCalculation(_scene, _target);
}

bool Skill::AccuracyCheck(BattleScene* _scene, Combatant* _target) const {
	uint16_t _accuracyBuff	= uint16_t(_scene->curCombatant->GetCurrentStatModifier(ACCURACY_MODIFIER) - _target->GetCurrentStatModifier(EVASION_MODIFIER));
	uint16_t _accuracy		= accuracy;

	if (_accuracyBuff > 0ui16)		{ _accuracy *= (9ui16 + _accuracyBuff) / 9ui16; }
	else if (_accuracyBuff < 0ui16) { _accuracy *= 9ui16 / (9ui16 - _accuracyBuff); }

	bool _attackHit = (_accuracy >= std::rand() % 0xFFui16);
	if (!_attackHit)
		_scene->battleUI->CreateText("MISS", _scene->targets[_scene->curSkillTarget], COLOR_WHITE, 16.0f, 16.0f);
	return _attackHit;
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

void Skill::PhysicalDamageCalculation(BattleScene* _scene, Combatant* _target) {
	Combatant* _caster	= _scene->curCombatant;
	float_t _damage		= float_t(basePower);
	_damage				= (_damage / 2.0f) + (_damage * float_t(_caster->stats[STAT_STRENGTH]) / 15.0f);
	_damage				= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage				= _damage / std::pow(float_t(_target->stats[STAT_ENDURANCE]), 0.2f);
	_damage				= DamageRandomize(_damage); // Randomly select a value between 95% and 105% of the calculated damage.
	ResistanceEffect(_scene, _damage, _target);
}

void Skill::MagicDamageCalculation(BattleScene* _scene, Combatant* _target) {
	Combatant* _caster	= _scene->curCombatant;
	float_t _damage		= float_t(basePower);
	_damage				= _damage + (_damage * float_t(_caster->stats[STAT_MAGIC]) / 25.0f);
	_damage				= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage				= _damage / std::pow(float_t(_target->stats[STAT_INTELLIGENCE]), 0.2f);
	_damage				= DamageRandomize(_damage); // Randomly select a value between 95% and 105% of the calculated damage.
	ResistanceEffect(_scene, _damage, _target);
}

void Skill::VoidDamageCalculation(BattleScene* _scene, Combatant* _target) {
	Combatant* _caster	= _scene->curCombatant;
	float_t _damage		= float_t(basePower);
	_damage				= _damage + (_damage * std::sqrt(float_t(_caster->stats[STAT_STRENGTH] + _caster->stats[STAT_MAGIC]) / 3.0f));
	_damage				= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage				= _damage / std::pow(float_t(_target->stats[STAT_ENDURANCE] + _target->stats[STAT_INTELLIGENCE]), 0.1f);
	_damage				= DamageRandomize(_damage); // Randomly select a value between 95% and 105% of the calculated damage.
	ResistanceEffect(_scene, _damage, _target);
}

void Skill::ResistanceEffect(BattleScene* _scene, float_t _damage, Combatant* _target) const {
	auto& _resists	= _target->resistances;
	uint8_t _effect = 0ui8;
	for (size_t i = 0ui64; i < MAIN_AFFINITY_COUNT; i++) {
		if (_resists[i].first == affinity) {
			_effect = _resists[i].second;
			break; // Once the affinity's effect value is found, exit the loop.
		}
	}

	if (_effect & OVERWRITE_RESIST_MASK)	{ _effect = (_effect & OVERWRITE_RESIST_MASK) >> 4; }
	else									{ _effect = (_effect & BASE_RESIST_MASK); }

	int16_t _finalDamage	= 0ui16;
	BattleUI* _battleUI		= _scene->battleUI;
	size_t _index			= _scene->targets[_scene->curSkillTarget];
	switch (_effect) { // Determine what to do to the damage value relative to how the target is affected by the skill's affinity.
	case EFFECT_BREAK:
		_finalDamage = int16_t(_damage * 2.0f);
		_battleUI->CreateText("BREAK", _index, COLOR_LIGHT_YELLOW, 16.0f, 8.0f);
		_battleUI->CreateDamageText(_finalDamage, _index, COLOR_WHITE, 16.0f, 16.0f);
		break;
	case EFFECT_WEAK:
		_finalDamage = int16_t(_damage * 1.5f);
		_battleUI->CreateText("WEAK", _index, COLOR_YELLOW, 16.0f, 8.0f);
		_battleUI->CreateDamageText(_finalDamage, _index, COLOR_WHITE, 16.0f, 16.0f);
		break;
	case EFFECT_RESIST:
		_finalDamage = int16_t(_damage * 0.5f);
		_battleUI->CreateText("RESIST", _index, COLOR_LIGHT_RED, 16.0f, 8.0f);
		_battleUI->CreateDamageText(_finalDamage, _index, COLOR_WHITE, 16.0f, 16.0f);
		break;
	case EFFECT_NULL:
		_battleUI->CreateText("NULL", _index, COLOR_DARK_RED, 16.0f, 8.0f);
		break;
	case EFFECT_ABSORB:
		_finalDamage = -int16_t(_damage);
		_battleUI->CreateDamageText(-_finalDamage, _index, COLOR_GREEN, 16.0f, 16.0f);
		break;
	case EFFECT_REFLECT:
		
		break;
	}

	if (_finalDamage != 0i16) {
		_battleUI->uiElements[_index]->ShowElement(1.0f);
		_scene->UpdateHitpoints(_target, _finalDamage);
	}
}