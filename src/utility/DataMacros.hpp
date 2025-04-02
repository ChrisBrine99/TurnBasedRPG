#ifndef DATA_MACROS_HPP
#define DATA_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Some defines for the amount of area reserved in memory for character,skill, and graphical data, as well as another macro that		//
//	determines how many slots are added to each data type's respective structure when the capacity has been hit/exceeded.				//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define DATA_CHAR_RESERVE_SIZE			32ui64
#define DATA_SKILL_RESERVE_SIZE			64ui64
#define DATA_SPRITE_RESERVE_SIZE		16ui64
#define DATA_RESERVE_CHUNK_SIZE			16ui64

#define ADD_DATA(_storage, _id, _data)									\
			size_t _capacity = _storage.bucket_count();					\
			if (_storage.size() == _capacity)							\
				_storage.reserve(_capacity + DATA_RESERVE_CHUNK_SIZE);	\
			_storage[_id] = _data										\

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
#define KEY_RESISTANCES					"resists"
#define KEY_BASIC_ATTACK				"bAtk"
// --- Keys that are unique to enemy characters ---- //
#define KEY_ENEMIES						"enemies"
#define KEY_EXP_REWARD					"exp"
#define KEY_MONEY_REWARD				"money"
#define KEY_ITEM_REWARDS				"items"
#define KEY_ITEM_CHANCES				"chances"
#define KEY_MAXIMUM_HP					"hp"
#define KEY_MAXIMUM_MP					"mp"
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
#define CHR_GREEN_SLIME					0x0000ui16
#define CHR_RED_SLIME					0x0001ui16
// --- The boundary value between what is considered a enemy character or a player character --- //
#define CHR_ID_BOUNDARY					0xF000ui16
// --- Master list of playable character IDs --- //
#define CHR_TEST_PLAYER					0xF001ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the unique ID values for each skill within the game.															//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define SKL_IGNIA						0x0000ui16
#define SKL_POLIGNIA					0x0010ui16
#define SKL_PLAYER_BASIC_ATK_0			0xF000ui16
#define SKL_ENEMY_BASIC_ATK_0			0xF001ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the unique ID values for each spritesheet within the game.													//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define SPR_PLAYER						0x0000ui16

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Simple default values that represents an invalid ID within the various data structures in the game. As a result, they must all be	//
//	16-bit values ranging from 0-65534 AKA no more than 65535 unique pieces of data can exist for any one group.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define CHR_INVALID						0xFFFFui16
#define SKL_INVALID						0xFFFFui16
#define SPR_INVALID						0xFFFFui16
#define ENC_INVALID						0xFFFFui16

#endif