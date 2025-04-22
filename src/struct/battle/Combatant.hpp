#ifndef COMBATANT_HPP
#define COMBATANT_HPP

#include "../character/BaseCharacter.hpp"

struct ActiveSkill;

struct Combatant {
	uint8_t					level;						// Simply stores a copy of the character's level at the time the battle began; ranging from 1-100.
	std::array<uint8_t, CHR_STAT_COUNT>					// Stores the current calculated values for the character's seven major stats at the beginning of the battle.
							stats;						
	uint16_t				curHitpoints;				// Tracks the current HP for the character throughout the battle.
	uint16_t				maxHitpoints;				// The maximum HP of the character at the time the battle began.
	float_t					maxHitpointMultiplier;		// Multiplicative bonus to the character's current maximum HP that only applies for the current battle.
	uint16_t				curMagicpoints;				// Tracks the current MP for the character throughout the battle.
	uint16_t				maxMagicpoints;				// The maximum MP of the character at the time the battle began.
	float_t					maxMagicpointMultiplier;	// Multiplicative bonus to the character's current maximum MP that only applies for the current battle.
	std::vector<uint16_t>	activeSkills;				// A copy of the vector within the character instance that stores the skills they can use within a battle.
	uint8_t					curNerveAilment;			// Stores the index value current nerve ailment that the character is afflicted with.
	uint8_t					curMindAilment;				// Stores the index value for the current mind ailment that the character is afflicted with.
	uint8_t					curSpecialAilment;			// Stores the current special ailment that is affecting the character (Ex. crippled for a turn).
	bool					isActive;					// A single flag that denotes if the combatant is currently participating in the battle or not.
	uint16_t				baseSpeed;					// A battle-only stat that is derived from a formula utilizing the character's level and agility stat. Determines their position within the turn order.
	uint16_t				statModifiers;				// Current stat modifiers that are applied to the character. These can be: attack, defense, accuracy, evasion, and speed.
	BaseCharacter*			character;					// A pointer to the character this combatant represents in case any data not found here needs to be referenced.
	std::array<Affinity, MAIN_AFFINITY_COUNT>			// Stores a pair of values: the index value for the affinity, and the character's resistance/weakness to it.
							resistances;				
	ActiveSkill*			basicAttack;				// A pointer to the character's basic attack, which isn't found within the data structure containing normal active/passive skills.

public: // Constructor/Destructor Declarations
	Combatant();
	Combatant(const Combatant& _other) = delete;
	~Combatant() = default;

public: // Publicly Accessible Utility Function Declarations
	void ActivateCombatant(BaseCharacter* _character);
	bool InflictStatusAilment(uint8_t _ailmentIndex);

	inline uint16_t GetMaxHitpoints()	const { return uint16_t(maxHitpoints * maxHitpointMultiplier); }
	inline uint16_t GetMaxMagicpoints() const { return uint16_t(maxMagicpoints * maxMagicpointMultiplier); }

	// Isolates the bits within the "statModifiers" variable that represent the current modifier values for each of the 
	// Combatant's battle stats. In this state they range between 0 and 7, but they will be offset by -3 to actually be
	// within the range of -3 to +4, which is what is required by the BattleManager in order to execute the proper formulas 
	// for each respective battle stat should they need to affect that particular aspect of the character's performance in
	// the current battle.
	inline int8_t GetCurrentStatModifier(uint16_t _modifierBitMask) const {
		switch (_modifierBitMask) {
		case ATTACK_MODIFIER:		return int8_t( statModifiers & ATTACK_MODIFIER );
		case DEFENCE_MODIFIER:		return int8_t((statModifiers & DEFENCE_MODIFIER)	>> 3);
		case ACCURACY_MODIFIER:		return int8_t((statModifiers & ACCURACY_MODIFIER)	>> 6);
		case EVASION_MODIFIER:		return int8_t((statModifiers & EVASION_MODIFIER)	>> 9);
		case SPEED_MODIFIER:		return int8_t((statModifiers & SPEED_MODIFIER)		>> 12);
		default:					return 3i8;
		}
	}
};

#endif