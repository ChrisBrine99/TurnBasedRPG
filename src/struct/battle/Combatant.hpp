#ifndef COMBATANT_HPP
#define COMBATANT_HPP

#include "../character/BaseCharacter.hpp"

struct Combatant {
	uint8_t							level;
	std::array<uint8_t, STAT_COUNT> stats;

	uint16_t	curHitpoints;
	uint16_t	maxHitpoints;
	float_t		maxHitpointMultiplier;

	uint16_t	curMagicpoints;
	uint16_t	maxMagicpoints;
	float_t		maxMagicpointMultiplier;

	std::vector<uint16_t> activeSkills;

	uint8_t		curNerveAilment;
	uint8_t		curMindAilment;
	uint8_t		curSpecialAilment;

	bool		isActive;

	uint16_t	baseSpeed;
	uint16_t	statModifiers;

	BaseCharacter*	character;

	std::array<Affinity, MAIN_AFFINITY_COUNT> resistances;

	uint16_t	basicAttack;

public: // Constructor/Destructor Declarations
	Combatant();
	Combatant(const Combatant& _other) = delete;
	~Combatant() = default;

public: // Publicly Accessible Utility Function Declarations
	void ActivateCombatant(BaseCharacter* _character);
	bool InflictStatusAilment(uint8_t _ailmentIndex);

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