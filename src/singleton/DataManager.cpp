#include "DataManager.hpp"
INIT_SINGLETON_CPP(DataManager)

#include "../utility/BattleMacros.hpp"
#include "../utility/CharacterMacros.hpp"
#include "../struct/skill/ActiveSkill.hpp"
#include "../struct/skill/PassiveSkill.hpp"
#include "../struct/skill/Skill.hpp"
#include "../struct/character/EnemyCharacter.hpp"
#include "../struct/character/PlayerCharacter.hpp"
#include "../struct/object/Animation.hpp"
#include "../utility/Logger.hpp"

DataManager::DataManager() :
	skills(),
	characters(),
	sprites(),
	animations()
{
	characters.reserve(DATA_CHAR_RESERVE_SIZE);
	skills.reserve(DATA_SKILL_RESERVE_SIZE);
	sprites.reserve(DATA_SPRITE_RESERVE_SIZE);
}

bool DataManager::OnUserCreate() {
	return true;
}

bool DataManager::OnUserDestroy() {
	RemoveDataFromStorage(characters);
	RemoveDataFromStorage(skills);
	RemoveDataFromStorage(sprites);
	RemoveDataFromStorage(animations);
	return true;
}

bool DataManager::OnBeforeUserCreate() {
	uint16_t _id = 0ui16; // This value is used for loading in all ID-based data. Resets back to 0 after each group has loaded.
	json _skills = json::parse(std::ifstream("res/data/skills.json"));

	// Load in all active skills first. Skipping over malformed data within the JSON object.
	for (auto& _aSkill : _skills[KEY_ACTIVE_SKILL_LIST]) {
		if (!_aSkill.is_null()) { LoadSkillData(_id++, _aSkill); }
	}

	// Load in all passive skills once active skills have all loaded. This means all IDs for passive skills will be
	// higher than ANY active skill's id if that distinction should ever be required.
	for (auto& _pSkill : _skills[KEY_PASSIVE_SKILL_LIST]) {
		if (!_pSkill.is_null()) { LoadSkillData(_id++, _pSkill, true); }
	}

	// Unload the JSON data now that it isn't required.
	_skills.clear();

	characterData	= json::parse(std::ifstream("res/data/characters.json"));
	encounterData	= json::parse(std::ifstream("res/data/encounters.json"));
	animationData	= json::parse(std::ifstream("res/data/animations.json"));

	return true;
}

BaseCharacter* DataManager::LoadCharacterData(uint16_t _id) {
	// Don't attempt to load in a character that already exists; simply return the exist character instead.
	if (characters.find(_id) != characters.end())
		return characters[_id];
	std::string _idString = std::to_string(_id);

	// The supplied ID is above the boundary between player and enemy characters, so the data will be interpreted as if the
	// character is an player even if the data is supposed to represent an enemy.
	if (_id >= CHR_ID_BOUNDARY) {
		// Try to find the container for the character's data. Exit and return nullptr if the required container doesn't exist.
		json& _data = characterData[KEY_FRIENDLIES][_idString];
		if (_data.is_null())
			return nullptr;
		PlayerCharacter* _newPlayerChar = new PlayerCharacter(_id);

		// Set the proper ID value to be occupied by this new player character instance and then begin loading in the data
		// that is shared between both enemies and players.
		ADD_DATA(characters, _id, _newPlayerChar);
		LoadSharedCharacterData(_id, _data);
		
		// Make sure the player character's known skills match up with what their active skills vector currently contains.
		// After that, another loops begins to parse all the known skills into said vector.
		uint16_t _skillID	= CHR_INVALID;
		json& _knownSkills	= _data[KEY_KNOWN_SKILLS];
		_newPlayerChar->knownSkills.insert(_newPlayerChar->knownSkills.begin(), _newPlayerChar->activeSkills.begin(), _newPlayerChar->activeSkills.end());
		_newPlayerChar->knownSkills.reserve(_newPlayerChar->knownSkills.size() + _knownSkills.size());
		for (size_t i = 0ui64; i < _knownSkills.size(); i++) {
			_skillID = _knownSkills[i];
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
	EnemyCharacter* _newEnemyChar = new EnemyCharacter(_id);
	ADD_DATA(characters, _id, _newEnemyChar);
	LoadSharedCharacterData(_id, _data);

	// Assign the proper AI function to the enemy that it will utilize in battle.
	SetEnemyAIFunction(_newEnemyChar, _data[KEY_ENEMY_AI]);

	// Unique to enemy character's is a maximum HP and MP value that aren't calculated by a formula, so they're set here by
	// simply reading the contents of that data within the JSON object. The current HP and MP are also set to those maximums.
	_newEnemyChar->maxHitpointBase		= uint16_t(_data[KEY_MAXIMUM_HP]);
	_newEnemyChar->curHitpoints			= _newEnemyChar->GetMaxHitpointsTotal();
	_newEnemyChar->maxMagicpointBase	= uint16_t(_data[KEY_MAXIMUM_MP]);
	_newEnemyChar->curMagicpoints		= _newEnemyChar->GetMaxMagicpointsTotal();

	// Copy over the experience and money rewards upon the defeat of the enemy in battle.
	_newEnemyChar->expReward			= uint16_t(_data[KEY_EXP_REWARD]);
	_newEnemyChar->moneyReward			= uint16_t(_data[KEY_MONEY_REWARD]);

	// Load in the enemy character's item drop information, which stores the item IDs and their respective drop chances as a
	// pair. If this data doesn't exist within the enemy character's json object, the enemy is simply returned without that data.
	json& _itemRewards = _data[KEY_ITEM_REWARDS];
	json& _itemChances = _data[KEY_ITEM_CHANCES];
	if (_itemRewards.is_null() || _itemChances.is_null())
		return _newEnemyChar;

	for (size_t i = 0ui64; i < _itemRewards.size(); i++) // Add all potential item rewards and their chances out of 255.
		_newEnemyChar->itemRewards.push_back(std::make_pair(uint16_t(_itemRewards[i]), uint8_t(_itemChances[i])));
	return _newEnemyChar;
}

inline void DataManager::LoadSharedCharacterData(uint16_t _id, json& _data) {
	BaseCharacter* _character		= characters[_id];
	std::string_view _name			= std::string_view(_data[KEY_NAME]).substr(0ui64, CHR_EFFECTIVE_NAME_SIZE);
	_name.copy(&_character->name[0ui64], CHR_EFFECTIVE_NAME_SIZE);
	_character->level				= uint8_t(_data[KEY_LEVEL]);

	// Load in each of the character's seven major stats from where they're stored in the JSON data.
	auto& _stats					= _character->statBase;
	_stats[CHR_STAT_STRENGTH]		= uint8_t(_data[KEY_STRENGTH]);
	_stats[CHR_STAT_MAGIC]			= uint8_t(_data[KEY_MAGIC]);
	_stats[CHR_STAT_ENDURANCE]		= uint8_t(_data[KEY_ENDURANCE]);
	_stats[CHR_STAT_INTELLIGENCE]	= uint8_t(_data[KEY_INTELLIGENCE]);
	_stats[CHR_STAT_AGILITY]		= uint8_t(_data[KEY_AGILITY]);
	_stats[CHR_STAT_CONCENTRATION]	= uint8_t(_data[KEY_CONCENTRATION]);
	_stats[CHR_STAT_LUCK]			= uint8_t(_data[KEY_LUCK]);

	// Load in the character's active skills. This is automatically limited to 6 skills for playable characters.
	json& _innerData	= _data[KEY_ACTIVE_SKILLS];
	size_t _length		= (_id > CHR_ID_BOUNDARY) ? std::min(_innerData.size(), PCHR_SKILL_LIMIT) : _innerData.size();
	uint16_t _skillID	= CHR_INVALID;
	for (size_t i = 0ui64; i < _length; i++) {
		_skillID = _innerData[i];
		_character->activeSkills.push_back(_skillID);
	}

	// Load in the character's base resistances into the "resistances" array as a pair containing the internal id value 
	// for the affinity alongside the effect that affinity will have on the character.
	_innerData		= _data[KEY_RESISTANCES];
	_length			= _innerData.size();
	auto& _resists	= _character->resistances;
	for (size_t j = 0ui64; j < _length; j++)
		_resists[j] = std::make_pair(BaseCharacter::resistIndex[j], _innerData[j]);
}

void DataManager::LoadSkillData(uint16_t _id, const json& _data, bool _isPassive) {
	if (_isPassive) {
		PassiveSkill* _newPassiveSkill		= new PassiveSkill();
		ADD_DATA(skills, _id, _newPassiveSkill);
		LoadSharedSkillData(_id, _data);

		_newPassiveSkill->maxHpBonus		= int16_t(_data[KEY_PSKILL_HPBONUS]);
		_newPassiveSkill->maxMpBonus		= int16_t(_data[KEY_PSKILL_MPBONUS]);
		_newPassiveSkill->maxHpMultiplier	= uint8_t(_data[KEY_PSKILL_HPMULTIPLIER]);
		_newPassiveSkill->maxMpMultiplier	= uint8_t(_data[KEY_PSKILL_MPMULTIPLIER]);
		_newPassiveSkill->affinityFlags		= uint16_t(_data[KEY_PSKILL_AFFINITY_FLAGS]);
		_newPassiveSkill->affinityModifier	= uint8_t(_data[KEY_PSKILL_AFFINITY_MOD]);
		_newPassiveSkill->buffDurationBonus = uint8_t(_data[KEY_PSKILL_BUFF_DURATION_BONUS]);
		_newPassiveSkill->critChanceBonus	= uint8_t(_data[KEY_PSKILL_CRIT_CHANCE_BONUS]);
		_newPassiveSkill->critDamageBonus	= uint8_t(_data[KEY_PSKILL_CRIT_DAMAGE_BONUS]);
		_newPassiveSkill->resistOverwrite	= uint8_t(_data[KEY_PSKILL_RESIST_VALUE]);
		_newPassiveSkill->useSkillID		= uint16_t(_data[KEY_PSKILL_ASKILL_ID]);
		return;
	}

	ActiveSkill* _newActiveSkill	= new ActiveSkill();
	ADD_DATA(skills, _id, _newActiveSkill);
	LoadSharedSkillData(_id, _data);

	_newActiveSkill->hpCost			= uint16_t(_data[KEY_ASKILL_HPCOST]);
	_newActiveSkill->mpCost			= uint16_t(_data[KEY_ASKILL_MPCOST]);
	_newActiveSkill->basePower		= uint16_t(_data[KEY_ASKILL_POWER]);
	_newActiveSkill->accuracy		= uint8_t(_data[KEY_ASKILL_ACCURACY]);
	_newActiveSkill->hitCount		= uint8_t(_data[KEY_ASKILL_NUMHITS]);
	_newActiveSkill->critChance		= uint8_t(_data[KEY_ASKILL_CRIT_CHANCE]);
	_newActiveSkill->critBonus		= uint8_t(_data[KEY_ASKILL_CRIT_BONUS]);
	// TODO -- Add recoil power section to skills.json for each skill
	_newActiveSkill->addedEffects		= {
		uint8_t(_data[KEY_ASKILL_EFFECT1]),
		uint8_t(_data[KEY_ASKILL_EFFECT2]),
		uint8_t(_data[KEY_ASKILL_EFFECT3]),
		uint8_t(_data[KEY_ASKILL_EFFECT4])
	};
	_newActiveSkill->effectChance	= uint8_t(_data[KEY_ASKILL_EFFECT_CHANCE]);
	_newActiveSkill->flags			= uint32_t(_data[KEY_ASKILL_FLAGS]);
	_newActiveSkill->buffAmount		= uint16_t(_data[KEY_ASKILL_BUFF_AMOUNT]);
	_newActiveSkill->buffDuration	= uint8_t(_data[KEY_ASKILL_BUFF_DURATION]);
	_newActiveSkill->dmgMultiplier	= uint8_t(_data[KEY_ASKILL_DAMAGE_MULT]);
	_newActiveSkill->healPower		= uint16_t(_data[KEY_ASKILL_HEAL_POWER]);
	
	switch (uint16_t(_data[KEY_SKILL_USE_ID])) {
	default:							_newActiveSkill->useFunction = nullptr;									return;
	case SKILL_PHYSICAL_GENERIC:		_newActiveSkill->useFunction = &ActiveSkill::UsePhysicalSkillGeneric;	return;
	case SKILL_MAGICAL_GENERIC:			_newActiveSkill->useFunction = &ActiveSkill::UseMagicSkillGeneric;		return;
	case SKILL_MAGICAL_PLUS_EFFECT:		_newActiveSkill->useFunction = &ActiveSkill::UseMagicSkillPlusEffect;	return;
	case SKILL_PHYSMAG_GENERIC:			_newActiveSkill->useFunction = &ActiveSkill::UseVoidSkillGeneric;		return;
	case SKILL_HEALING_GENERIC:			_newActiveSkill->useFunction = &ActiveSkill::UseHealingSkillGeneric;	return;
	}
}

void DataManager::LoadSharedSkillData(uint16_t _id, const json& _data) {
	Skill* _skill			= skills[_id];
	std::string_view _name	= std::string_view(_data[KEY_SKILL_NAME]).substr(0ui64, SKILL_EFFECTIVE_NAME_SIZE);
	_name.copy(&_skill->name[0ui64], SKILL_EFFECTIVE_NAME_SIZE); // Max is one character less since the 16th element IS ALWAYS a null-termination character.
	_skill->id				= _id;
	_skill->affinity		= _data[KEY_SKILL_AFFINITY];
	_skill->targeting		= _data[KEY_SKILL_TARGET];
}

olc::Sprite* DataManager::LoadSprite(uint16_t _id, const std::string& _filepath) {
	// Don't attempt to load in a sprite that already exists. Simply return the existing sprite's pointer.
	if (sprites.find(_id) != sprites.end())
		return sprites[_id];

	// Don't try loading in the sprite if the file provided didn't contain valid image data.
	olc::Sprite* _newSprite = new olc::Sprite();
	if (_newSprite->LoadFromFile(_filepath) != olc::OK) {
		delete _newSprite, _newSprite = nullptr;
		return nullptr;
	}

	// Finally, add the sprite to the data manager's list of loaded sprites, and then return its pointer.
	ADD_DATA(sprites, _id, _newSprite);
	return sprites[_id];
}

Animation* DataManager::LoadAnimation(uint16_t _id) {
	// Don't attempt to load in an animation that already exists. Simply return the existing animation's pointer.
	if (animations.find(_id) != animations.end())
		return animations[_id];

	// Don't try loading in an animation if the relevant data couldn't be retrieved at the specified ID.
	json& _data = animationData[std::to_string(_id)];
	if (_data.is_null() || !GetSprite(_data[KEY_ANIM_SPRITE_ID]))
		return nullptr;

	// Create a new animation instance, and copy over the width, height, and id for the animation.
	Animation* _newAnimation	= new Animation();
	_newAnimation->width		= _data[KEY_ANIM_WIDTH];
	_newAnimation->height		= _data[KEY_ANIM_HEIGHT];
	_newAnimation->id			= _id;

	// Attempt to retrieve the animation's frame data array (This array should always been an evan value size since it 
	// requires an x and y value for each frame of the animation.
	json& _frameData			= _data[KEY_ANIM_FRAME_DATA];
	if (_frameData.is_null())
		return nullptr;
	
	// Make sure the size is even. If it isn't, the animation data is invalid and will not be loaded. If it is, the numFrames
	// variable will be set to the length of the frame data array divided by two.
	size_t _size = _frameData.size();
	if ((_size % 2ui64) != 0ui64) {
		delete _newAnimation, _newAnimation = nullptr;
		return nullptr;
	}
	_newAnimation->numFrames	= uint16_t(_size / 2ui64);

	// Finally, the data is looped through and places into the animation's vector for frame position data.
	auto& _animFrameData		= _newAnimation->frameData;
	_animFrameData.reserve(_size);
	for (int32_t i : _frameData)
		_animFrameData.push_back(i);

	ADD_DATA(animations, _id, _newAnimation);
	return animations[_id];
}

inline void DataManager::SetEnemyAIFunction(EnemyCharacter* _enemy, uint16_t _id) {
	switch (_id) {
	default:
	case ENEMY_AI_SIMPLE:				_enemy->battleAI = &EnemyCharacter::EnemySimpleAI;		return;
	}
}

template<class T>
inline void DataManager::RemoveDataFromStorage(std::unordered_map<uint16_t, T>& _storage) {
	for (auto& _pair : _storage)
		delete _pair.second, _pair.second = nullptr;
	_storage.clear();
}