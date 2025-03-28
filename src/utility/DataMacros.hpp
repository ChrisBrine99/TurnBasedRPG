#ifndef DATA_MACROS_HPP
#define DATA_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Macros that hold key values for each element found within a character's raw JSON data. Certain values are unique to only enemy or	//
//	friendly characters, respectively.																									//
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
#define KEY_EXP_REWARD					"exp"
#define KEY_MONEY_REWARD				"money"
#define KEY_ITEM_REWARDS				"items"
#define KEY_ITEM_CHANCES				"chances"
#define KEY_MAXIMUM_HP					"hp"
#define KEY_MAXIMUM_MP					"mp"
#define KEY_BASIC_ATTACK				"bAtk"
#define KEY_ENEMY_AI					"ai"
// --- Keys that are unique to player characters --- //
#define KEY_FRIENDLIES					"friendlies"
#define KEY_EQUIPMENT					"equip"
#define KEY_KNOWN_SKILLS				"kSkills"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Macros that hold key values for each element found within a skill's raw JSON data.													//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define KEY_SKILL_NAME					"name"
#define KEY_SKILL_INFO					"info"
#define KEY_SKILL_TYPE					"type"
#define KEY_SKILL_TARGET				"target"
#define KEY_SKILL_HP_COST				"hCost"
#define KEY_SKILL_MP_COST				"mCost"
#define KEY_SKILL_POWER					"power"
#define KEY_SKILL_ACCURACY				"acc"
#define KEY_SKILL_MINIMUM_HITS			"minHit"
#define KEY_SKILL_MAXIMUM_HITS			"maxHit"
#define KEY_SKILL_EFFECTS				"effects"
#define KEY_SKILL_EFFECT_CHANCE			"chance"
#define KEY_SKILL_USE_FUNCTION			"use"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Macros that hold key values for each element found within an encounter's raw JSON data.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define KEY_ENCOUNTER_ENEMIES			"enemies"
#define KEY_ENCOUNTER_SPAWN_COUNT		"max"
#define KEY_ENCOUNTER_SPAWN_CHANCE		"spawn"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the unique ID values for each character/enemy within the game.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

// --- Master list of enemy character IDs --- //
#define ID_GREEN_SLIME					0x0000ui16
#define ID_RED_SLIME					0x0001ui16
// --- The boundary value between what is considered a enemy character or a player character --- //
#define ID_BOUNDARY						0xF000ui16
// --- Master list of playable character IDs --- //
#define ID_TEST_PLAYER					0xF001ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the unique ID values for each skill within the game.															//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define ID_IGNIA						0x0000ui16
#define ID_POLIGNIA						0x0010ui16

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A simple default value that represents an invalid ID within the various data structures in the game. As a result, they must all be	//
//	16-bit values ranging from 0-65534 AKA no more than 65535 unique pieces of data can exist for any one group.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define ID_INVALID						0xFFFFui16

#endif