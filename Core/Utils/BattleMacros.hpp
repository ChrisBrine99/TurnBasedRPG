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

#define STATE_BATTLE_INITIALIZE			0ui8
#define STATE_BATTLE_SET_TURN_ORDER		1ui8
#define STATE_BATTLE_CHECK_TURN_TYPE	2ui8
#define STATE_BATTLE_PLAYER_TURN		3ui8
#define STATE_BATTLE_ENEMY_TURN			4ui8
#define STATE_BATTLE_IS_ROUND_DONE		5ui8
#define STATE_BATTLE_WIN				6ui8
#define STATE_BATTLE_LOSE				7ui8
#define STATE_BATTLE_ESCAPE				8ui8
#define STATE_POST_BATTLE				9ui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_BATTLE_ACTIVE				0x00000001u

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_IS_BATTLE_ACTIVE			(flags & FLAG_BATTLE_ACTIVE)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Values for the bits that represent a certain characteristic within a given Combatant struct. Allows 32 boolean values to be stored	//
//	in 4 bytes of space instead of 32 bytes.																							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_COMBATANT_PLAYER			0x00000001u
#define FLAG_COMBATANT_ACTIVE			0x00000002u

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that allow a specific Combatant's flags to be checked for the state of each flag found within its "flags" variable.			//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_IS_COMBATANT_PLAYER(_x)	(_x->flags & FLAG_COMBATANT_PLAYER)
#define FLAG_IS_COMBATANT_ACTIVE(_x)	(_x->flags & FLAG_COMBATANT_ACTIVE)

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
#define AFFINITY_VOID					0x30ui8

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
// --- Targets all allies and enemies; including self --- //
#define TARGET_EVERYONE					0x09ui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Since all modifier values are packed into a single 16-bit value, the unique values can be grabbed/modified through each of these	//
//	define statements. Each individual value is an unsigned 3-bit value that ranges from 0 to +7 which is then offset by -3 to			//
//	calcluate the final stat value after the modification.																				// 
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define ATTACK_MODIFIER					0x0007ui16
#define DEFENCE_MODIFIER				0x0038ui16
#define	ACCURACY_MODIFIER				0x01C0ui16
#define EVASION_MODIFIER				0x0E00ui16
#define SPEED_MODIFIER					0x7000ui16

#endif