#include "BaseCharacter.hpp"
#include "../skill/ActiveSkill.hpp"

std::array<uint8_t, MAIN_AFFINITY_COUNT> BaseCharacter::resistIndex = {
	AFFINITY_PHYSICAL,
	AFFINITY_FIRE,
	AFFINITY_WATER,
	AFFINITY_AIR,
	AFFINITY_EARTH,
	AFFINITY_SHOCK,
	AFFINITY_FROST,
	AFFINITY_LIGHT,
	AFFINITY_DARK,
	AFFINITY_VOID,
	AFFINITY_POISON,
	AFFINITY_SLEEP,
	AFFINITY_CHARM,
	AFFINITY_SILENCE,
	AFFINITY_CONFUSION
};

BaseCharacter::BaseCharacter(uint16_t _id) :
	name('\0'),
	level(0ui8),
	statBase({ 1ui8, 1ui8, 1ui8, 1ui8, 1ui8, 1ui8, 1ui8 }),
	statBonus({ 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8 }),
	statMultiplier({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }),
	curHitpoints(0ui16),
	curMagicpoints(0ui16),
	maxHitpointBase(0ui16),
	maxHitpointBonus(0i16),
	maxHitpointMultiplier(1.0f),
	maxMagicpointBase(0ui16),
	maxMagicpointBonus(0i16),
	maxMagicpointMultiplier(1.0f),
	activeSkills(),
	id(_id),
	basicAttack(nullptr)
{ // Initialize default resistance values and also reserve a small chunk of memory for the character's pool of usable skills. Also create basic attack skill if it doesn't exist.
	for (size_t i = 0ui64; i < MAIN_AFFINITY_COUNT; i++)
		resistances[i] = std::make_pair(resistIndex[i], EFFECT_NORMAL);

	activeSkills.reserve(PCHR_SKILL_LIMIT);
	SetBasicAttackAttributes(_id);
}

BaseCharacter::~BaseCharacter() {
	activeSkills.clear();
	if (basicAttack) { delete basicAttack, basicAttack = nullptr; }
}

void BaseCharacter::SetBasicAttackAttributes(uint16_t _id) {
	if (basicAttack)
		return;
	basicAttack = new ActiveSkill();

	switch (_id) {
	default: // General basic attack for all characters.
		basicAttack->id				= SKL_BASIC_ATTACK;
		basicAttack->affinity		= AFFINITY_PHYSICAL;
		basicAttack->targeting		= TARGET_SINGLE_ENEMY;
		basicAttack->basePower		= 20ui8;
		basicAttack->accuracy		= 240ui8;	// ~ 94.12%
		basicAttack->critChance		= 16ui8;	// ~  6.27%
		basicAttack->hitCount		= 17ui8;	// 00010001 in binary AKA a single hit
		basicAttack->useFunction	= &ActiveSkill::UsePhysicalSkillGeneric;
		break;
	}
}