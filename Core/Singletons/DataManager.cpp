#include "DataManager.hpp"
INIT_SINGLETON_CPP(DataManager)

#include "../Utils/BattleMacros.hpp"
#include "../Utils/CharacterMacros.hpp"

#include "../Structs/Characters/PlayerCharacter.hpp"
#include "../Structs/Characters/EnemyCharacter.hpp"
#include "../Structs/Battle/Skill.hpp"

#include <filesystem>

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

	return true;
}

bool DataManager::OnBeforeUserCreate() {
	std::ifstream _file("Resources/characters.json");
	characterData = json::parse(_file);

	LoadCharacterData(ID_TEST_PLAYER);
	LoadCharacterData(ID_TEST_ENEMY);

	return true;
}

void DataManager::LoadCharacterData(uint16_t _id) {
	// Don't attempt to load in a character that already exists.
	if (characters.find(_id) != characters.end())
		return;
	std::string _idString = std::to_string(_id);

	// The supplied ID is above the boundary between player and enemy characters, so the data will be interpreted as if the
	// character is an player even if the data is supposed to represent an enemy.
	if (_id >= ID_BOUNDARY) {
		// Try to find the container for the character's data. Exit and return nullptr if the required container doesn't exist.
		json& _data = characterData[KEY_FRIENDLIES][_idString];
		if (_data.is_null())
			return;
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
		// as their Endurance and Magic stats, respectively.
		_newPlayerChar->maxHitpointBase		= _newPlayerChar->CalculateMaxBaseHitpoints();
		_newPlayerChar->maxMagicpointBase	= _newPlayerChar->CalculateMaxBaseMagicpoints();

		// TODO -- Load in equipment item IDs here.
	}

	// Try to find the container for the character's data. Exit and return nullptr if the required container doesn't exist.
	json& _data = characterData[KEY_ENEMIES][_idString];
	if (_data.is_null())
		return;
	
	// Do the same as above, but for an enemy character that is being added instead of a player one.
	EnemyCharacter* _newEnemyChar = new EnemyCharacter();
	characters[_id] = _newEnemyChar;
	LoadSharedCharacterData(_id, _data);

	// Unique to enemy character's is a maximum HP and MP value that aren't calculated by a formula, so they're set here by
	// simply reading the contents of that data within the JSON object.
	_newEnemyChar->maxHitpointBase		= uint16_t(_data[KEY_MAXIMUM_HP]);
	_newEnemyChar->maxMagicpointBase	= uint16_t(_data[KEY_MAXIMUM_MP]);

	// Another element unique to an enemy is a dedicated basic attack, which is set in the same way the above elements were.
	_newEnemyChar->basicAttack			= uint16_t(_data[KEY_BASIC_ATTACK]);
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
	size_t _length		= (_id >= ID_BOUNDARY) ? PLAYER_SKILL_LIMIT : _skillData.size();
	uint16_t _skillID	= ID_INVALID;
	for (size_t i = 0ui64; i < _length; i++) {
		_skillID = _skillData[i];
		if (skills.find(_skillID) == skills.end())
			break;
		_character->activeSkills.push_back(_skillID);
	}
}

template<class T>
inline void DataManager::RemoveDataFromStorage(std::unordered_map<uint16_t, T>& _storage) {
	for (auto& _pair : _storage)
		delete _pair.second, _pair.second = nullptr;
	_storage.clear();
}