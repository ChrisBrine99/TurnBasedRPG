#include "GameSettings.hpp"
INIT_SINGLETON_CPP(GameSettings)

#include <fstream>

GameSettings::GameSettings() :
	volume(),
	inputBindings()
{}

bool GameSettings::OnBeforeUserCreate() {
	// Defaults for the game's volume settings.
	volume[AUDIO_MASTER_VOLUME]				= 0.9f;
	volume[AUDIO_MUSIC_VOLUME]				= 0.75f;
	volume[AUDIO_SOUND_EFFECT_VOLUME]		= 0.85f;

	// Defaults for the game's input bindings.
	inputBindings[INPUT_MOVE_RIGHT]			= olc::Key::RIGHT;	// In-Game Character Movement
	inputBindings[INPUT_MOVE_LEFT]			= olc::Key::LEFT;
	inputBindings[INPUT_MOVE_UP]			= olc::Key::UP;
	inputBindings[INPUT_MOVE_DOWN]			= olc::Key::DOWN;
	inputBindings[INPUT_INTERACT]			= olc::Key::Z;		// In-Game Action Inputs
	inputBindings[INPUT_MENU_RIGHT]			= olc::Key::RIGHT;	// Menu Cursor Movement
	inputBindings[INPUT_MENU_LEFT]			= olc::Key::LEFT;
	inputBindings[INPUT_MENU_UP]			= olc::Key::UP;
	inputBindings[INPUT_MENU_DOWN]			= olc::Key::DOWN;
	inputBindings[INPUT_MENU_SELECT]		= olc::Key::Z;		// General Menu Interaction Inputs
	inputBindings[INPUT_MENU_RETURN]		= olc::Key::X;
	inputBindings[INPUT_SAVE_FILE_COPY]		= olc::Key::C;		// Context-specific Menu Inputs
	inputBindings[INPUT_SAVE_FILE_DELETE]	= olc::Key::V;

	// After all default values have been set, attempt loading in a pre-existing "settings.ini" file to overwrite 
	// those defaults with values that have been saved by the program to store the user's preferences.
	LoadSettings();

	return true;
}

bool GameSettings::OnUserDestroy() {
	return true;
}

void GameSettings::LoadSettings(const std::string& _filename) {
	std::ifstream _file(_filename);
	if (!_file.is_open())
		return; // "settings.ini" doesn't exist; don't even attempt reading from it.
	
	// Credit to javidx9 for this starting/ending whitespace removal lambda function
	auto _trimWhitespace = [](std::string& _string) {
		_string.erase(0, _string.find_first_not_of(" \t\n\r\f\v"));
		_string.erase(_string.find_last_not_of(" \t\n\r\f\v") + 1);
	};

	size_t _position;
	std::string _curSection;
	std::string _line;
	while (!_file.eof()) {
		std::getline(_file, _line);

		// Make sure the actual line being read was more than just whitespace. Otherwise, move onto the next line.
		_trimWhitespace(_line);
		if (_line.empty()) {
			_curSection.clear();
			continue; // Always assume the current section is complete when there is an empty line.
		}

		// Parse the line relative to the current section that is being read through.
		if (!_curSection.empty()) {
			if (_curSection == SETTINGS_GROUP_INPUT) { LoadInputSetting(_line); }
			continue;
		}

		// Determine if a new section is being read, and store which section exactly so it can be referenced 
		// while reading subsequent lines of text.
		_position = _line.find_first_of("[");
		if (_position != std::string::npos)
			_curSection = _line;
	}
	_file.close();
}

void GameSettings::SaveSettings(const std::string& _filename) {
	std::ofstream _file(_filename);
	if (_file.is_open()) {
		// Writing all input bindings into the file; having their key and value stored together seperated by '='.
		_file << SETTINGS_GROUP_INPUT << '\n';
		for (auto& _data : inputBindings)
			_file << _data.first << '=' << _data.second << '\n';

		_file.close();
	}
}

void GameSettings::LoadInputSetting(const std::string& _data){
	size_t _position = _data.find_first_of('=');
	if (_position == std::string::npos) 
		return; // No way to differentiate key from value; don't load input binding.

	std::string _key = _data.substr(0, _position);
	if (inputBindings.find(_key) == inputBindings.end())
		return; // No valid input binding exists for the key found; don't load the input.

	std::string _value = _data.substr(_position + 1);
	if (_value.find_first_not_of("0123456789") != std::string::npos)
		return; // The value specified isn't a number, meaning it cannot be used as an input.

	inputBindings[_key] = (olc::Key) std::stoi(_value);
}

olc::HWButton GameSettings::GetKeyInfo(const char* _key) const {
	if (inputBindings.find(_key) == inputBindings.end())
		return olc::HWButton{ false, false, false };
	return GET_SINGLETON(EngineCore)->GetKey(inputBindings.at(_key));
}