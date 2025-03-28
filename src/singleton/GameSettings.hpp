#ifndef GAME_SETTINGS_HPP
#define GAME_SETTINGS_HPP

#include "../utility/GeneralMacros.hpp"

#include <string>
#include <unordered_map>

namespace olc {
	struct HWButton;
	enum Key;
}

class GameSettings {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(GameSettings)
public: // Main Engine Function Declarations
	bool OnBeforeUserCreate();
	bool OnUserDestroy();

public: // Accessible Utility Function Decarations
	olc::HWButton GetKeyInfo(const char* _key) const;
	void LoadSettings(const std::string& _filename = "settings.ini");
	void SaveSettings(const std::string& _filename = "settings.ini");

private: // Hidden Utility Function Declarations
	void LoadInputSetting(const std::string& _data);

private: // Hidden Member Variable Declarations
	std::unordered_map<std::string, olc::Key>	inputBindings;
	std::unordered_map<std::string, float_t>	volume;
};

#endif