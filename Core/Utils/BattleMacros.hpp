#ifndef BATTLE_MACROS_HPP
#define BATTLE_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Two values that represent the maximum amount of enemies that can exist within a battle at any given time as well as the total		//
//	amount of player characters that can be active in a battle at any given time.														//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BATTLE_MAX_ENEMY_SIZE			8ui64
#define BATTLE_MAX_PARTY_SIZE			3ui64
#define BATTLE_TOTAL_COMBATANTS			BATTLE_MAX_ENEMY_SIZE + BATTLE_MAX_PARTY_SIZE

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Makes sense of the numbers that represent the BattleManager's various states.													//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define STATE_BATTLE_INITIALIZE			0x00ui8
#define STATE_BATTLE_SET_TURN_ORDER		0x01ui8
#define STATE_BATTLE_CHECK_TURN_TYPE	0x02ui8
#define STATE_BATTLE_PLAYER_TURN		0x03ui8
#define STATE_BATTLE_ENEMY_TURN			0x04ui8
#define STATE_BATTLE_EXECUTE_SKILL		0x05ui8
#define STATE_BATTLE_IS_ROUND_DONE		0x10ui8
#define STATE_BATTLE_WIN				0x11ui8
#define STATE_BATTLE_LOSE				0x12ui8
#define STATE_BATTLE_ESCAPE				0x13ui8
#define STATE_BATTLE_POST				0x14ui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the bits that can be used within the battle manager's "flags" variable to enable or disable certain			//
//	functionalities of itself without overwriting any state function is may be executing.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_BATTLE_ACTIVE				0x00000001u
#define FLAG_BATTLE_WAIT_ANIMATION		0x00000002u

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that simplify the formatting required to check for each of the battle manager's state-independent flags.					//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define BATTLE_IS_ACTIVE				(flags & FLAG_BATTLE_ACTIVE)
#define BATTLE_PAUSED_FOR_ANIMATION		(flags & FLAG_BATTLE_WAIT_ANIMATION)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Values for the bits that represent a certain characteristic within a given Combatant struct. Allows 32 boolean values to be stored	//
//	in 4 bytes of space instead of 32 bytes.																							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_COMBATANT_PLAYER			0x00000001u
#define FLAG_COMBATANT_ACTIVE			0x00000002u

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that allow a specific Combatant's flags to be checked for the state of each flag found within its "flags" variable.			//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define COMBATANT_IS_PLAYER(_x)			(_x->flags & FLAG_COMBATANT_PLAYER)
#define COMBATANT_IS_ACTIVE(_x)			(_x->flags & FLAG_COMBATANT_ACTIVE)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines for each of the game's affinities. These are utilized by skills to determine how they will function and how they will		//
//	affect the target(s) upon use.																										//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define AFFINITY_INVALID				0xFFui8
// --- Strength-based affinities --- //
#define AFFINITY_PHYSICAL				0x00ui8
// --- Magic-based affinities --- //
#define AFFINITY_FIRE					0x10ui8
#define AFFINITY_WATER					0x11ui8
#define AFFINITY_AIR					0x12ui8
#define AFFINITY_EARTH					0x13ui8
#define AFFINITY_SHOCK					0x14ui8
#define AFFINITY_FROST					0x15ui8
// --- Special Magic-based affinities --- //
#define AFFINITY_LIGHT					0x20ui8
#define AFFINITY_DARK					0x21ui8
// --- Strength AND Magic-based affinities --- //
#define AFFINITY_VOID					0x80ui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that will determine how a skill will function within a battle with regards to the targets it can affect when used.			//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define TARGET_INVALID					0xFFui8
// --- Targets a single enemy/ally/self --- //
#define TARGET_SELF						0x00ui8
#define TARGET_SINGLE_ENEMY				0x01ui8
#define TARGET_SINGLE_ALLY				0x02ui8
#define TARGET_SINGLE_ENEMY_SELF		0x03ui8
#define TARGET_SINGLE_ALLY_SELF			0x04ui8
// --- Targets all combatants on one side of the field --- //
#define TARGET_ALL_ENEMY				0x05ui8
#define TARGET_ALL_ALLY					0x06ui8
#define TARGET_ALL_ENEMY_SELF			0x07ui8
#define TARGET_ALL_ALLY_SELF			0x08ui8
// --- Targets all allies and enemies --- //
#define TARGET_EVERYONE					0x09ui8
#define TARGET_EVERYONE_SELF			0x0Aui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the internal ID values for each ailment within the game. They are grouped into three distinct groups: mind,	//
//	nerve, and special; each group having ranked priority based on which ailment has the smaller index in each group.					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

// --- Nerve Group Status Ailments --- //
#define AILMENT_NERVE_NONE				0x6Fui8
#define AILMENT_NERVE_FREEZE			0x00ui8
#define AILMENT_NERVE_PARALYSIS			0x01ui8
#define AILMENT_NERVE_BURN				0x02ui8
#define AILMENT_NERVE_POISON			0x03ui8
#define AILMENT_NERVE_SLEEP				0x04ui8
// --- Mind Group Status Ailments --- //
#define AILMENT_MIND_NONE				0xDFui8
#define AILMENT_MIND_CHARM				0x70ui8
#define AILMENT_MIND_CURSE				0x71ui8
#define AILMENT_MIND_BLIND				0x72ui8
#define AILMENT_MIND_SILENCE			0x73ui8
#define AILMENT_MIND_CONFUSION			0x74ui8
// --- Special Status Ailments --- //
#define AILMENT_SPECIAL_NONE			0xFEui8
#define AILMENT_SPECIAL_CRIPPLE			0xE0ui8
// --- Invalid Value for Ailment Data --- //
#define AILMENT_INVALID					0xFFui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Value that determines how many unique status conditions a skill may possibly inflict upon the target(s) when used.					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define SKILL_MAX_UNIQUE_EFFECTS		4ui64

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Since all modifier values are packed into a single 16-bit value, the unique values can be grabbed/modified through each of these	//
//	define statements. Each individual value is an unsigned 3-bit value that ranges from 0 to +7 which is then offset by -3 to			//
//	calcluate the final stat value after the modification.																				// 
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define ATTACK_MODIFIER					0x0007ui16	// 00000000 00000111
#define DEFENCE_MODIFIER				0x0038ui16	// 00000000 00111000
#define	ACCURACY_MODIFIER				0x01C0ui16	// 00000001 11000000
#define EVASION_MODIFIER				0x0E00ui16	// 00001110 00000000
#define SPEED_MODIFIER					0x7000ui16	// 01110000 00000000
// The sixteenth bit in the variable is unused.

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that determine the function that the skill will execute when used in a battle.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define SKILL_PHYSICAL_GENERIC			0x0000ui16
#define SKILL_FIRE_GENERIC				0x0100ui16
#define SKILL_WATER_GENERIC				0x0200ui16
#define SKILL_AIR_GENERIC				0x0300ui16
#define SKILL_EARTH_GENERIC				0x0400ui16
#define SKILL_SHOCK_GENERIC				0x0500ui16
#define SKILL_FROST_GENERIC				0x0600ui16
#define SKILL_LIGHT_GENERIC				0x0700ui16
#define SKILL_DARK_GENERIC				0x0800ui16
#define SKILL_VOID_GENERIC				0x0900ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that determine the function that an enemy will use during its turn in a given battle.										//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define	ENEMY_AI_SIMPLE					0x0000ui16

#endif