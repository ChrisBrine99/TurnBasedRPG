#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include "../../ThirdParty/json.hpp"
#include "../Utils/GeneralMacros.hpp"
#include "../Utils/DataMacros.hpp"

#include <fstream>
#include <unordered_map>

using json = nlohmann::json;

struct BaseCharacter;
struct EnemyCharacter;
struct Skill;

class DataManager {
	// Singleton Initialization/Constructor Declaration
	INIT_SINGLETON_HPP(DataManager)
	DataManager();
public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();

	bool OnBeforeUserCreate();

public: // Accessible Utility Function Declarations
	void LoadCharacterData(uint16_t _id);
	void LoadSkillData(uint16_t _id);

private: // Hidden Utility Function Declarations
	inline void LoadSharedCharacterData(uint16_t _id, json& _data);
	inline void SetEnemyAIFunction(EnemyCharacter* _enemy, uint16_t _id);
	inline void SetSkillUseFunction(Skill* _skill, uint16_t _id);

	template<class T>
	inline void RemoveDataFromStorage(std::unordered_map<uint16_t, T>& _storage);

public: // Getters and Setters for data held by this singleton
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

	inline json& GetEncounterData(uint16_t _id) {
		return encounterData[std::to_string(_id)];
	}
private: // Hidden Member Variable Declarations
	std::unordered_map<uint16_t, BaseCharacter*>	characters;
	std::unordered_map<uint16_t, Skill*>			skills;

	json characterData;
	json skillData;
	json encounterData;
};

#endif