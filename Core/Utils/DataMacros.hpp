#ifndef DATA_MACROS_HPP
#define DATA_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Macros that hold key values for each element found within a character's data. Certain values are unique to only enemy or friendly	//
//	characters, respectively.																											//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define KEY_NAME						"name"
#define KEY_LEVEL						"level"
#define KEY_STRENGTH					"str"
#define KEY_MAGIC						"mag"
#define KEY_ENDURANCE					"end"
#define KEY_INTELLIGENCE				"int"
#define KEY_AGILITY						"agi"
#define KEY_CONCENTRATION				"con"
#define KEY_LUCK						"luk"
#define KEY_ACTIVE_SKILLS				"aSkills"
// --- Keys that are unique to enemy characters ---- //
#define KEY_ENEMIES						"enemies"
#define KEY_MAXIMUM_HP					"hp"
#define KEY_MAXIMUM_MP					"mp"
#define KEY_BASIC_ATTACK				"bAtk"
// --- Keys that are unique to player characters --- //
#define KEY_FRIENDLIES					"friendlies"
#define KEY_EQUIPMENT					"equip"
#define KEY_KNOWN_SKILLS				"kSkills"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the unique ID values for each character/enemy within the game.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

// --- Master list of enemy character IDs --- //
#define ID_TEST_ENEMY					0x0000ui16
// --- The boundary value between what is considered a enemy character or a player character --- //
#define ID_BOUNDARY						0xF000ui16
// --- Master list of playable character IDs --- //
#define ID_TEST_PLAYER					0xF001ui16

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A simple default value that represents an invalid ID within the various data structures in the game. As a result, they must all be	//
//	16-bit values ranging from 0-65534 AKA no more than 65535 unique pieces of data can exist for any one group.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define ID_INVALID						0xFFFFui16

#endif