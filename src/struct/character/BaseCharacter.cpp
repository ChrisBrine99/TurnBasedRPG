#include "BaseCharacter.hpp"

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

BaseCharacter::BaseCharacter() :
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
	basicAttack(ID_INVALID)
{ // Initialize default resistance values and also reserve a small chunk of memory for the character's pool of usable skills.
	for (size_t i = 0ui64; i < MAIN_AFFINITY_COUNT; i++)
		resistances[i] = std::make_pair(resistIndex[i], EFFECT_NORMAL);
	activeSkills.reserve(PLAYER_SKILL_LIMIT);
}