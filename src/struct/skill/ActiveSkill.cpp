#include "ActiveSkill.hpp"

#include <cmath>
#include <iostream>

#include "../../scene/BattleScene.hpp"
#include "../../ui/battle/BattleUI.hpp"
#include "../../ui/battle/BattleUIElement.hpp"
#include "../battle/Combatant.hpp"

ActiveSkill::ActiveSkill() : 
	Skill(),
	basePower(0ui16),
	accuracy(0ui8),
	hpCost(0ui16),
	mpCost(0ui16),
	hitCount(0ui8),
	critChance(0ui8),
	critBonus(0ui8),
	recoilPower(0ui8),
	addedEffects({ AILMENT_INVALID, AILMENT_INVALID, AILMENT_INVALID, AILMENT_INVALID }),
	effectChance(0ui8),
	buffAmount(0x36DBui16),
	buffDuration(0ui8),
	dmgMultiplier(0ui8),
	healPower(0ui16),
	useFunction(nullptr)
{}

void ActiveSkill::ExecuteUseFunction(BattleScene* _scene, Combatant* _target) {
	if (useFunction == nullptr)
		return;
	((*this).*(this->useFunction))(_scene, _target);
}

void ActiveSkill::UsePhysicalSkillGeneric(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene, _target))
		return;
	PhysicalDamageCalculation(_scene, _target);
}

void ActiveSkill::UseMagicSkillGeneric(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene, _target))
		return;
	MagicDamageCalculation(_scene, _target);
}

void ActiveSkill::UseVoidSkillGeneric(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene, _target))
		return;
	VoidDamageCalculation(_scene, _target);
}

void ActiveSkill::UseHealingSkillGeneric(BattleScene* _scene, Combatant* _target) {
	float_t _healAmount = float_t(healPower);
	HealingEffect(_scene, (_healAmount * 0.5f) + (_healAmount * _scene->curCombatant->stats[CHR_STAT_MAGIC] * 0.25f), _target);
}

void ActiveSkill::UseMagicSkillPlusEffect(BattleScene* _scene, Combatant* _target) {
	if (!AccuracyCheck(_scene, _target))
		return;
	AdditionalEffectCheck(_target); // Attempt to apply one of the skill's possible status ailments on top of damaging the target.
	MagicDamageCalculation(_scene, _target);
}

bool ActiveSkill::AccuracyCheck(BattleScene* _scene, Combatant* _target) const {
	if (_scene->curCombatant == _target)
		return true; // Skills used on self (Or recoiled back to self) ignore accuracy checks.

	uint16_t _accuracyBuff	= uint16_t(_scene->curCombatant->GetCurrentStatModifier(ACCURACY_MODIFIER) - _target->GetCurrentStatModifier(EVASION_MODIFIER));
	uint16_t _accuracy		= accuracy;

	if (_accuracyBuff > 0ui16)		{ _accuracy *= (9ui16 + _accuracyBuff) / 9ui16; } 
	else if (_accuracyBuff < 0ui16) { _accuracy *= 9ui16 / (9ui16 - _accuracyBuff); }

	bool _attackHit = (_accuracy >= std::rand() % 0xFFui16);
	if (!_attackHit) {
		size_t _index = _scene->targets[_scene->curSkillTarget];
		_scene->battleUI->CreateText("MISS", 0.0f, 60.0f, COLOR_WHITE, BattleScene::positions[_index].first + 16.0f, BattleScene::positions[_index].second + 16.0f);
	}
	return _attackHit;
}

bool ActiveSkill::CriticalCheck(BattleScene* _scene, Combatant* _target) const {

}

void ActiveSkill::AdditionalEffectCheck(Combatant* _target) {
	//for (size_t i = 0ui64; i < SKILL_MAX_UNIQUE_EFFECTS; i++) {
	//	if (addedEffects[i] == AILMENT_INVALID)
	//		continue;
	//
	//	// The earlier effects in the list will always have higher priority than the effects at the end. However, the later effects
	//	// will always be considered if the higher priority effects fail to successfully affect the target(s).
	//	if (effectChance[i] >= uint8_t(std::rand() % 0xFFui8) && _target->InflictStatusAilment(addedEffects[i]))
	//		return;
	//}
}

void ActiveSkill::PhysicalDamageCalculation(BattleScene* _scene, Combatant* _target) {
	Combatant* _caster	= _scene->curCombatant;
	float_t _damage		= float_t(basePower);
	_damage				= (_damage / 2.0f) + (_damage * float_t(_caster->stats[CHR_STAT_STRENGTH]) / 15.0f);
	_damage				= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage				= _damage / std::pow(float_t(_target->stats[CHR_STAT_ENDURANCE]), 0.2f);
	_damage				= DamageRandomize(_damage); // Randomly select a value between 95% and 105% of the calculated damage.
	ResistanceEffect(_scene, _damage, _target);
}

void ActiveSkill::MagicDamageCalculation(BattleScene* _scene, Combatant* _target) {
	Combatant* _caster	= _scene->curCombatant;
	float_t _damage		= float_t(basePower);
	_damage				= _damage + (_damage * float_t(_caster->stats[CHR_STAT_MAGIC]) / 25.0f);
	_damage				= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage				= _damage / std::pow(float_t(_target->stats[CHR_STAT_INTELLIGENCE]), 0.2f);
	_damage				= DamageRandomize(_damage); // Randomly select a value between 95% and 105% of the calculated damage.
	ResistanceEffect(_scene, _damage, _target);
}

void ActiveSkill::VoidDamageCalculation(BattleScene* _scene, Combatant* _target) {
	Combatant* _caster	= _scene->curCombatant;
	float_t _damage		= float_t(basePower);
	_damage				= _damage + (_damage * std::sqrt(float_t(_caster->stats[CHR_STAT_STRENGTH] + _caster->stats[CHR_STAT_MAGIC]) / 3.0f));
	_damage				= _damage * DamageBuffCalculation(_caster->GetCurrentStatModifier(ATTACK_MODIFIER) - _target->GetCurrentStatModifier(DEFENCE_MODIFIER));
	_damage				= _damage / std::pow(float_t(_target->stats[CHR_STAT_ENDURANCE] + _target->stats[CHR_STAT_INTELLIGENCE]), 0.1f);
	_damage				= DamageRandomize(_damage); // Randomly select a value between 95% and 105% of the calculated damage.
	ResistanceEffect(_scene, _damage, _target);
}

void ActiveSkill::ResistanceEffect(BattleScene* _scene, float_t _damage, Combatant* _target) const {
	auto& _resists = _target->resistances;
	uint8_t _effect = 0ui8;
	for (size_t i = 0ui64; i < MAIN_AFFINITY_COUNT; i++) {
		if (_resists[i].first == affinity) {
			_effect = _resists[i].second;
			break; // Once the affinity's effect value is found, exit the loop.
		}
	}

	if (_effect & OVERWRITE_RESIST_MASK) { _effect = (_effect & OVERWRITE_RESIST_MASK) >> 4; } else { _effect = (_effect & BASE_RESIST_MASK); }

	int16_t _finalDamage	= 0ui16;
	BattleUI* _battleUI		= _scene->battleUI;
	size_t _index			= _scene->targets[_scene->curSkillTarget];
	olc::vf2d _position		= { BattleScene::positions[_index].first + 16.0f, BattleScene::positions[_index].second + 16.0f };
	switch (_effect) { // Determine what to do to the damage value relative to how the target is affected by the skill's affinity.
	default:
		_finalDamage = int16_t(_damage);
		break;
	case EFFECT_BREAK:
		_finalDamage = int16_t(_damage * 2.0f);
		_battleUI->CreateText("BREAK", -0.3f, 60.0f, COLOR_LIGHT_YELLOW, _position.x, _position.y - 8.0f);
		break;
	case EFFECT_WEAK:
		_finalDamage = int16_t(_damage * 1.5f);
		_battleUI->CreateText("WEAK", -0.3f, 60.0f, COLOR_YELLOW, _position.x, _position.y - 8.0f);
		break;
	case EFFECT_RESIST:
		_finalDamage = int16_t(_damage * 0.5f);
		_battleUI->CreateText("RESIST", -0.3f, 60.0f, COLOR_LIGHT_RED, _position.x, _position.y - 8.0f);
		break;
	case EFFECT_NULL:
		_battleUI->CreateText("NULL", 0.0f, 60.0f, COLOR_DARK_RED, _position.x, _position.y - 8.0f);
		break;
	case EFFECT_ABSORB:
		_finalDamage = -int16_t(_damage);
		break;
	case EFFECT_REFLECT:
		if (_scene->curCombatant == _target) {
			_battleUI->CreateText("NULL", 0.0f, 60.0f, COLOR_DARK_RED, _position.x, _position.y - 8.0f);
			break;
		}
		_battleUI->CreateText("REFLECT", 0.0f, 60.0f, COLOR_DARK_RED, _position.x, _position.y - 8.0f);
		_scene->targets.push_back(_scene->turnOrder[_scene->curTurn]);
		break;
	}

	if (_finalDamage == 0i16)
		return; // Target takes no damage, exit the function before dealing with any hp update function calls.

	if (_finalDamage > 0i16) { // Target will lose hp equal to the damage calcualted.
		olc::Pixel _color		= COLOR_WHITE; // Color changes as the target's hp gets lower.
		int16_t _nextHitpoints	= _target->curHitpoints - _finalDamage;

		if (_nextHitpoints / float_t(_target->maxHitpoints) < 0.33f)		{ _color = COLOR_LIGHT_RED; } 
		else if (_nextHitpoints / float_t(_target->maxHitpoints) < 0.5f)	{ _color = COLOR_LIGHT_YELLOW; }

		_battleUI->CreateText(std::to_string(_finalDamage).c_str(), -0.3f, 60.0f, _color, _position.x, _position.y);
	} else if (_finalDamage < 0i16) { // Target will be healed instead of taking damage.
		_battleUI->CreateText(std::to_string(std::abs(_finalDamage)).c_str(), 0.0f, 60.0f, COLOR_LIGHT_GREEN, _position.x, _position.y);
	}

	_battleUI->uiElements[_index]->ShowElement(60.0f);
	_scene->UpdateHitpoints(_target, _finalDamage);
}

void ActiveSkill::HealingEffect(BattleScene* _scene, float_t _healAmount, Combatant* _target) const {
	int16_t _finalHeal	= int16_t(_healAmount);
	BattleUI* _battleUI = _scene->battleUI;
	size_t _index		= _scene->targets[_scene->curSkillTarget];
	olc::vf2d _position = { BattleScene::positions[_index].first + 16.0f, BattleScene::positions[_index].second + 16.0f };

	_battleUI->CreateText(std::to_string(_finalHeal).c_str(), 0.0f, 60.0f, COLOR_LIGHT_GREEN, _position.x, _position.y);
	_scene->UpdateHitpoints(_target, -_finalHeal);
}