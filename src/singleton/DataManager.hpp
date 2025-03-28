#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include "../../third_party/olcPixelGameEngine.hpp"
#include "../../third_party/json.hpp"
#include "../utility/GeneralMacros.hpp"
#include "../utility/DataMacros.hpp"

#include <fstream>
#include <unordered_map>

using json = nlohmann::json;

struct BaseCharacter;
struct EnemyCharacter;
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
	Skill* LoadSkillData(uint16_t _id);

	olc::Decal* LoadSprite(uint16_t _id, const std::string& _filepath);
	void UnloadSprite(uint16_t _id);

private: // Hidden Utility Function Declarations
	inline void LoadSharedCharacterData(uint16_t _id, json& _data);
	inline void SetEnemyAIFunction(EnemyCharacter* _enemy, uint16_t _id);
	inline void SetSkillUseFunction(Skill* _skill, uint16_t _id);

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
		return sprites.at(_id).Sprite();
	}

	inline json& GetEncounter(uint16_t _id) {
		return encounterData[std::to_string(_id)];
	}
private: // Hidden Member Variable Declarations
	std::unordered_map<uint16_t, BaseCharacter*>	characters;
	std::unordered_map<uint16_t, Skill*>			skills;
	std::unordered_map<uint16_t, olc::Renderable>	sprites;

	json characterData;
	json skillData;
	json encounterData;
};

#endif