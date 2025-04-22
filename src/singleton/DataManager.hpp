#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include "../../third_party/olcPixelGameEngine.hpp"
#include "../../third_party/json.hpp"
#include "../utility/GeneralMacros.hpp"
#include "../utility/DataMacros.hpp"

#include <fstream>
#include <unordered_map>

using json = nlohmann::json;

struct Animation;
struct BaseCharacter;
struct EnemyCharacter;
struct ActiveSkill;
struct Skill;

class DataManager {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(DataManager)
public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();

	bool OnBeforeUserCreate();

public: // Accessible Utility Function Declarations
	BaseCharacter* LoadCharacterData(uint16_t _id);
	olc::Sprite* LoadSprite(uint16_t _id, const std::string& _filepath);
	Animation* LoadAnimation(uint16_t _id);

private: // Hidden Utility Function Declarations
	inline void LoadSharedCharacterData(uint16_t _id, json& _data);
	inline void SetEnemyAIFunction(EnemyCharacter* _enemy, uint16_t _id);

	void LoadSkillData(uint16_t _id, const json& _data, bool _isPassive = false);
	void LoadSharedSkillData(uint16_t _id, const json& _data);

	template<class T>
	inline void RemoveDataFromStorage(std::unordered_map<uint16_t, T>& _storage);

public: // Getter Function Declarations
	inline BaseCharacter* GetCharacter(uint16_t _id) const {
		if (characters.find(_id) == characters.end())
			return nullptr;
		return characters.at(_id);
	}

	inline Skill* GetSkill(uint16_t _id) const {
		if (skills.find(_id) == skills.end())
			return nullptr;
		return skills.at(_id);
	}

	inline olc::Sprite* GetSprite(uint16_t _id) {
		if (sprites.find(_id) == sprites.end())
			return nullptr;
		return sprites.at(_id);
	}

	inline json& GetEncounter(uint16_t _id) {
		return encounterData[std::to_string(_id)];
	}

	inline Animation* GetAnimation(uint16_t _id) {
		if (animations.find(_id) == animations.end())
			return nullptr;
		return animations.at(_id);
	}
private: // Hidden Member Variable Declarations
	std::unordered_map<uint16_t, BaseCharacter*>	characters;
	std::unordered_map<uint16_t, Skill*>			skills;
	std::unordered_map<uint16_t, olc::Sprite*>		sprites;
	std::unordered_map<uint16_t, Animation*>		animations;

	//json characterData;
	//json skillData;
	json encounterData;
	json animationData;
};

#endif