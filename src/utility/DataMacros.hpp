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

#define ADD_SPRITE(_id, _file)											\
			GET_SINGLETON(DataManager)->LoadSprite(_id, _file)

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
//	Macros that hold key values for each element found within a skill's data.															//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define KEY_SKILL_NAME					"name"
#define KEY_SKILL_AFFINITY				"affinity"
#define KEY_SKILL_TARGET				"target"
#define KEY_SKILL_USE_ID				"use id"
// --- Keys that are unique to active skills --- //
#define KEY_ACTIVE_SKILL_LIST			"active skills"
#define KEY_ASKILL_HPCOST				"hp cost"
#define KEY_ASKILL_MPCOST				"mp cost"
#define KEY_ASKILL_POWER				"power"
#define KEY_ASKILL_ACCURACY				"accuracy"
#define KEY_ASKILL_NUMHITS				"no. hits"
#define KEY_ASKILL_CRIT_CHANCE			"crit %"
#define KEY_ASKILL_CRIT_BONUS			"crit bonus"
#define KEY_ASKILL_EFFECT1				"effect 1"
#define KEY_ASKILL_EFFECT2				"effect 2"
#define KEY_ASKILL_EFFECT3				"effect 3"
#define KEY_ASKILL_EFFECT4				"effect 4"
#define KEY_ASKILL_EFFECT_CHANCE		"eff %"
#define KEY_ASKILL_FLAGS				"flags"
#define KEY_ASKILL_BUFF_AMOUNT			"buff amount"
#define KEY_ASKILL_BUFF_DURATION		"buff duration"
#define KEY_ASKILL_HEAL_POWER			"heal power"
#define KEY_ASKILL_DAMAGE_MULT			"dmg multiplier"
// --- Keys that are unique to passive skills --- //
#define KEY_PASSIVE_SKILL_LIST			"passive skills"
#define KEY_PSKILL_HPBONUS				"hp bonus"
#define KEY_PSKILL_MPBONUS				"mp bonus"
#define KEY_PSKILL_HPMULTIPLIER			"hp multiplier"
#define KEY_PSKILL_MPMULTIPLIER			"mp multiplier"
#define KEY_PSKILL_AFFINITY_FLAGS		"affinity flags"
#define KEY_PSKILL_AFFINITY_MOD			"affinity mod"
#define KEY_PSKILL_BUFF_DURATION_BONUS	"buff dur bonus"
#define KEY_PSKILL_CRIT_CHANCE_BONUS	"crit % bonus"
#define KEY_PSKILL_CRIT_DAMAGE_BONUS	"crit dmg bonus"
#define KEY_PSKILL_RESIST_VALUE			"resist value"
#define KEY_PSKILL_ASKILL_ID			"aSkill id"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Macros that hold key values for each element found within an encounter's raw JSON data.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define KEY_ENCOUNTER_ENEMIES			"enemies"
#define KEY_ENCOUNTER_SPAWN_COUNT		"max"
#define KEY_ENCOUNTER_SPAWN_CHANCE		"spawn"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Macros that hold key values for each element found within a animation's raw JSON data.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define KEY_ANIM_WIDTH					"width"
#define KEY_ANIM_HEIGHT					"height"
#define KEY_ANIM_FRAMES					"frames"
#define KEY_ANIM_FRAME_DATA				"data"
#define KEY_ANIM_SPRITE_ID				"sprID"

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
#define CHR_TEST_PLAYER_2				0xF002ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the unique ID values for each spritesheet within the game.													//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define SPR_PLAYER						0x0000ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the unique ID values for each encounter table within the game.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define ENC_TEST						0x0000ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the unique ID values for each sprite animation within the game.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define ANM_TEST						0x0000ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Define that is the value for the IDs of ALL basic attacks within the game, since they aren't stored/managed by the DataManager.		//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define SKL_BASIC_ATTACK				0xFFFEui16

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Simple default values that represents an invalid ID within the various data structures in the game. As a result, they must all be	//
//	16-bit values ranging from 0-65534 AKA no more than 65535 unique pieces of data can exist for any one group.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define CHR_INVALID						0xFFFFui16
#define SKL_INVALID						0xFFFFui16
#define SPR_INVALID						0xFFFFui16
#define ENC_INVALID						0xFFFFui16
#define ANM_INVALID						0xFFFFui16

#endif