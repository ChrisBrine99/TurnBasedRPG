#include "DataManager.hpp"
INIT_SINGLETON_CPP(DataManager)

#include "../utility/BattleMacros.hpp"
#include "../utility/CharacterMacros.hpp"
#include "../struct/character/PlayerCharacter.hpp"
#include "../struct/character/EnemyCharacter.hpp"
#include "../struct/battle/Skill.hpp"

DataManager::DataManager() :
	characterData(),
	characters(),
	skills()
{}

bool DataManager::OnUserCreate() {
	return true;
}

bool DataManager::OnUserDestroy() {
	RemoveDataFromStorage(characters);
	RemoveDataFromStorage(skills);
	sprites.clear();
	return true;
}

bool DataManager::OnBeforeUserCreate() {
	characterData	= json::parse(std::ifstream("resources/data/characters.json"));
	skillData		= json::parse(std::ifstream("resources/data/skills.json"));
	encounterData	= json::parse(std::ifstream("resources/data/encounters.json"));

	LoadSkillData(ID_IGNIA);
	LoadSkillData(ID_POLIGNIA);

	LoadCharacterData(ID_TEST_PLAYER);
	LoadCharacterData(ID_GREEN_SLIME);
	LoadCharacterData(ID_RED_SLIME);

	return true;
}

BaseCharacter* DataManager::LoadCharacterData(uint16_t _id) {
	// Don't attempt to load in a character that already exists.
	if (characters.find(_id) != characters.end())
		return nullptr;
	std::string _idString = std::to_string(_id);

	// The supplied ID is above the boundary between player and enemy characters, so the data will be interpreted as if the
	// character is an player even if the data is supposed to represent an enemy.
	if (_id >= ID_BOUNDARY) {
		// Try to find the container for the character's data. Exit and return nullptr if the required container doesn't exist.
		json& _data = characterData[KEY_FRIENDLIES][_idString];
		if (_data.is_null())
			return nullptr;
		PlayerCharacter* _newPlayerChar = new PlayerCharacter();

		// Set the proper ID value to be occupied by this new player character instance and then begin loading in the data
		// that is shared between both enemies and players.
		characters[_id]	= _newPlayerChar;
		LoadSharedCharacterData(_id, _data);
		
		// Make sure the player character's known skills match up with what their active skills vector currently contains.
		// After that, another loops begins to parse all the known skills into said vector.
		uint16_t _skillID	= ID_INVALID;
		json& _knownSkills	= _data[KEY_KNOWN_SKILLS];
		_newPlayerChar->knownSkills.insert(_newPlayerChar->knownSkills.begin(), _newPlayerChar->activeSkills.begin(), _newPlayerChar->activeSkills.end());
		_newPlayerChar->knownSkills.reserve(_newPlayerChar->knownSkills.size() + _knownSkills.size());
		for (size_t i = 0ui64; i < _knownSkills.size(); i++) {
			_skillID = _knownSkills[i];
			if (skills.find(_skillID) == skills.end())
				break;
			_newPlayerChar->knownSkills.push_back(_skillID);
		}

		// After the shared data has been initialized, the player character's starting exp and its next level experience
		// requirement will be calculated 
		_newPlayerChar->curExperience		= _newPlayerChar->DetermineRequiredExperience(_newPlayerChar->level);
		_newPlayerChar->nextLevelExperience = _newPlayerChar->DetermineRequiredExperience(_newPlayerChar->level + 1ui8);

		// Then initialize the starting base maximum HP and MP value since both require the player character's level as well
		// as their Endurance and Magic stats, respectively. The current HP and MP are also set to those calculated maximums.
		_newPlayerChar->maxHitpointBase		= _newPlayerChar->CalculateMaxBaseHitpoints();
		_newPlayerChar->curHitpoints		= _newPlayerChar->GetMaxHitpointsTotal();
		_newPlayerChar->maxMagicpointBase	= _newPlayerChar->CalculateMaxBaseMagicpoints();
		_newPlayerChar->curMagicpoints		= _newPlayerChar->GetMaxMagicpointsTotal();

		// TODO -- Load in equipment item IDs here.
		return _newPlayerChar;
	}

	// Try to find the container for the character's data. Exit and return nullptr if the required container doesn't exist.
	json& _data = characterData[KEY_ENEMIES][_idString];
	if (_data.is_null())
		return nullptr;
	
	// Do the same as above, but for an enemy character that is being added instead of a player one.
	EnemyCharacter* _newEnemyChar = new EnemyCharacter();
	characters[_id] = _newEnemyChar;
	LoadSharedCharacterData(_id, _data);

	// Unique to enemy character's is a maximum HP and MP value that aren't calculated by a formula, so they're set here by
	// simply reading the contents of that data within the JSON object. The current HP and MP are also set to those maximums.
	_newEnemyChar->maxHitpointBase		= uint16_t(_data[KEY_MAXIMUM_HP]);
	_newEnemyChar->curHitpoints			= _newEnemyChar->GetMaxHitpointsTotal();
	_newEnemyChar->maxMagicpointBase	= uint16_t(_data[KEY_MAXIMUM_MP]);
	_newEnemyChar->curMagicpoints		= _newEnemyChar->GetMaxMagicpointsTotal();

	// Another element unique to an enemy is a dedicated basic attack, which is set in the same way the above elements were.
	_newEnemyChar->basicAttack			= uint16_t(_data[KEY_BASIC_ATTACK]);

	// 
	_newEnemyChar->expReward			= uint16_t(_data[KEY_EXP_REWARD]);
	_newEnemyChar->moneyReward			= uint16_t(_data[KEY_MONEY_REWARD]);

	// 
	json& _itemRewards = _data[KEY_ITEM_REWARDS];
	json& _itemChances = _data[KEY_ITEM_CHANCES];
	if (_itemRewards.is_null() || _itemChances.is_null())
		return nullptr;

	for (size_t i = 0ui64; i < _itemRewards.size(); i++) // Add all potential item rewards and their chances out of 255.
		_newEnemyChar->itemRewards.insert(std::pair(uint16_t(_itemRewards[i]), uint8_t(_itemChances[i])));

	// Finally, assign the proper AI function to the enemy that it will utilize in battle.
	SetEnemyAIFunction(_newEnemyChar, _data[KEY_ENEMY_AI]);
	return _newEnemyChar;
}

Skill* DataManager::LoadSkillData(uint16_t _id) {
	// Don't attempt to load in a skill that already exists.
	if (skills.find(_id) != skills.end())
		return nullptr;

	// Don't try loading in a skill if the relevant data couldn't be retrieved at the specified ID.
	json& _data = skillData[std::to_string(_id)];
	if (_data.is_null())
		return nullptr;

	Skill* _newSkill		= new Skill();
	skills[_id]				= _newSkill;
	_newSkill->name			= _data[KEY_SKILL_NAME];
	_newSkill->description	= _data[KEY_SKILL_INFO];
	_newSkill->id			= _id;
	_newSkill->basePower	= _data[KEY_SKILL_POWER];
	_newSkill->affinity		= _data[KEY_SKILL_TYPE];
	_newSkill->accuracy		= _data[KEY_SKILL_ACCURACY];
	_newSkill->hpCost		= _data[KEY_SKILL_HP_COST];
	_newSkill->mpCost		= _data[KEY_SKILL_MP_COST];
	_newSkill->targeting	= _data[KEY_SKILL_TARGET];
	_newSkill->hitCount		= _data[KEY_SKILL_MINIMUM_HITS] | (_data[KEY_SKILL_MAXIMUM_HITS] << 4);
	_newSkill->addedEffects = _data[KEY_SKILL_EFFECTS];
	_newSkill->effectChance = _data[KEY_SKILL_EFFECT_CHANCE];
	SetSkillUseFunction(_newSkill, _data[KEY_SKILL_USE_FUNCTION]);

	return _newSkill;
}

olc::Decal* DataManager::LoadSprite(uint16_t _id, const std::string& _filepath) {
	if (sprites.find(_id) != sprites.end())
		return sprites[_id].Decal();

	if (sprites[_id].Load(_filepath) != olc::OK)
		return nullptr;

	return sprites[_id].Decal();
}

void DataManager::UnloadSprite(uint16_t _id) {
	if (sprites.find(_id) == sprites.end())
		return;
	sprites.erase(_id);
}

inline void DataManager::LoadSharedCharacterData(uint16_t _id, json& _data) {
	// It's assumed that the supplied ID is valid since this function is only ever called by the Data Manager when it
	// is reading its data from the JSON format into the actual Class that can be referenced/manipulated in the code.
	BaseCharacter* _character = characters[_id];

	// Loads in the character's given name (This can be changed for playable characters) as well as their level.
	_character->name							= _data[KEY_NAME];
	_character->level							= uint8_t(_data[KEY_LEVEL]);

	// Load in each of the character's seven major stats from where they're stored in the JSON data.
	_character->statBase[STAT_STRENGTH]			= uint8_t(_data[KEY_STRENGTH]);
	_character->statBase[STAT_MAGIC]			= uint8_t(_data[KEY_MAGIC]);
	_character->statBase[STAT_ENDURANCE]		= uint8_t(_data[KEY_ENDURANCE]);
	_character->statBase[STAT_INTELLIGENCE]		= uint8_t(_data[KEY_INTELLIGENCE]);
	_character->statBase[STAT_AGILITY]			= uint8_t(_data[KEY_AGILITY]);
	_character->statBase[STAT_CONCENTRATION]	= uint8_t(_data[KEY_CONCENTRATION]);
	_character->statBase[STAT_LUCK]				= uint8_t(_data[KEY_LUCK]);

	// Finally, load in the character's active skills. This is automatically limited to 6 skills for playable characters.
	json& _skillData	= _data[KEY_ACTIVE_SKILLS];
	size_t _length		= (_id > ID_BOUNDARY) ? std::min(_skillData.size(), PLAYER_SKILL_LIMIT) : _skillData.size();
	uint16_t _skillID	= ID_INVALID;
	for (size_t i = 0ui64; i < _length; i++) {
		_skillID = _skillData[i];
		if (skills.find(_skillID) == skills.end())
			break;
		_character->activeSkills.push_back(_skillID);
	}
}

inline void DataManager::SetEnemyAIFunction(EnemyCharacter* _enemy, uint16_t _id) {
	switch (_id) {
	default:
	case ENEMY_AI_SIMPLE:			_enemy->battleAI = &EnemyCharacter::EnemySimpleAI;		return;
	}
}

inline void DataManager::SetSkillUseFunction(Skill* _skill, uint16_t _id) {
	switch (_id) {
	default:
	case SKILL_PHYSICAL_GENERIC:	_skill->useFunction = &Skill::UsePhysicalSkillGeneric;	return;
	case SKILL_WATER_GENERIC:
	case SKILL_AIR_GENERIC:
	case SKILL_EARTH_GENERIC:		_skill->useFunction = &Skill::UseMagicSkillGeneric;		return;
	case SKILL_FIRE_GENERIC:		
	case SKILL_SHOCK_GENERIC:
	case SKILL_FROST_GENERIC:		
	case SKILL_LIGHT_GENERIC:		
	case SKILL_DARK_GENERIC:		_skill->useFunction = &Skill::UseMagicSkillPlusEffect;	return;
	case SKILL_VOID_GENERIC:		_skill->useFunction = &Skill::UseVoidSkillGeneric;		return;
	}
}

template<class T>
inline void DataManager::RemoveDataFromStorage(std::unordered_map<uint16_t, T>& _storage) {
	for (auto& _pair : _storage)
		delete _pair.second, _pair.second = nullptr;
	_storage.clear();
}