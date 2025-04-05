#include "Combatant.hpp"

Combatant::Combatant() :
	level(1ui8),
	stats(std::array<uint8_t, STAT_COUNT>()),
	curHitpoints(0ui16),
	maxHitpoints(0ui16),
	maxHitpointMultiplier(1.0f),
	curMagicpoints(0ui16),
	maxMagicpoints(0ui16),
	maxMagicpointMultiplier(1.0f),
	activeSkills(),
	curNerveAilment(AILMENT_NERVE_NONE),
	curMindAilment(AILMENT_MIND_NONE),
	curSpecialAilment(AILMENT_SPECIAL_NONE),
	isActive(false),
	baseSpeed(0ui16),
	statModifiers(0ui16),
	character(nullptr),
	resistances(std::array<Affinity, MAIN_AFFINITY_COUNT>()),
	basicAttack(SKL_INVALID) 
{
	stats.fill(1ui8); // Populate the array with default values of 1 for each stat.
	activeSkills.reserve(PLAYER_SKILL_LIMIT); // Reserve at least enough memory to store the maximum number of skills a player character can use in battle.
	resistances.fill(std::make_pair(AFFINITY_INVALID, EFFECT_NORMAL));
}

void Combatant::ActivateCombatant(BaseCharacter* _character) {
	level = _character->level; // Make sure the level is copied over.

	for (size_t i = 0ui64; i < STAT_COUNT; i++) // Copy over the seven main stat values.
		stats[i] = _character->GetCurrentStatTotal(uint8_t(i));

	// Get the character's current and maximum hitpoints.
	curHitpoints = _character->curHitpoints;
	maxHitpoints = _character->GetMaxHitpointsTotal();
	maxHitpointMultiplier = 1.0f; // Reset back to its default

	// Get the character's current and maximum magicpoints.
	curMagicpoints = _character->curMagicpoints;
	maxMagicpoints = _character->GetMaxMagicpointsTotal();
	maxMagicpointMultiplier = 1.0f; // Reset back to its default

	// Since the vector doesn't need to be copied, it can simply be referenced for the battle.
	activeSkills = _character->activeSkills;

	// Calculate/recalculate speed based on the character's level and agility stats.
	baseSpeed = uint16_t(level) + (uint16_t(stats[STAT_AGILITY]) * 2ui16) + 10ui16;

	// Stores the bits 0011 0110 1101 1011 which equates to each modifier being set to +3 which is actually
	// considered 0 by the BattleManager since it offsets the values by -3 during calculations.
	statModifiers = 0x36DBui16;

	// Flip the toggle that lets the battle manager know that the Combatant has been activated witin the battle, 
	// and store a pointer to the character they represent in case their data is required.
	isActive = true;
	character = _character;

	// Copy over the character's affinity resistance values but only either the base resistance or the overwritten 
	// value should there be one that exists for the character being initialized from.
	auto& _affinities = BaseCharacter::resistIndex;
	uint8_t _affinity = 0ui8;
	for (size_t i = 0ui64; i < MAIN_AFFINITY_COUNT; i++) {
		_affinity = _affinities[i];
		resistances[i] = std::make_pair(_affinity, _character->GetResistance(_affinity));
	}

	// Finally, copy over the ID value for the character's basic attack.
	basicAttack = _character->basicAttack;
}

bool Combatant::InflictStatusAilment(uint8_t _ailmentIndex) {
	if (_ailmentIndex < AILMENT_NERVE_NONE) {
		// The Combatant already has a nerve ailment inflicted upon them. Determine if this ailment is above the current
		// in priority and then perform a coin flip to determine if the new ailment will overwrite it.
		if (curNerveAilment != AILMENT_NERVE_NONE && (curNerveAilment <= _ailmentIndex || std::rand() % 2 == 0i32))
			return false; // Current ailment is already higher priority (Or the same ailment) OR the coin flip failed.

		curNerveAilment = _ailmentIndex;
		return true;
	}

	if (_ailmentIndex > AILMENT_NERVE_NONE && _ailmentIndex < AILMENT_MIND_NONE) {
		// Perform the same coin flip as above, but for the currently inflicted mind ailment vs the new ailment that can
		// potentially be inflicted if it's higher priority and the coin flip succeeds.
		if (curMindAilment != AILMENT_NERVE_NONE && (curMindAilment <= _ailmentIndex || std::rand() % 2 == 0i32))
			return false;

		curMindAilment = _ailmentIndex;
		return true;
	}

	// Unlike mind and nerve ailments, a special ailment will always overwrite the previously inflicted ailment.
	if (_ailmentIndex == AILMENT_MIND_NONE || _ailmentIndex == AILMENT_SPECIAL_NONE || curSpecialAilment == AILMENT_SPECIAL_NONE)
		return false;

	curSpecialAilment = _ailmentIndex;
	return true;
}