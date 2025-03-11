#ifndef GENERAL_MACROS_HPP
#define GENERAL_MACROS_HPP

#include "UtilityFunctions.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for colors that will be utilized all over the code's GUI elements.															//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define COLOR_WHITE					olc::Pixel(0xF8ui8, 0xF8ui8, 0xF8ui8)
#define COLOR_LIGHT_GRAY			olc::Pixel(0xBCui8, 0xBCui8, 0xBCui8)
#define COLOR_GRAY					olc::Pixel(0x7Cui8, 0x7Cui8, 0x7Cui8)
#define COLOR_DARK_GRAY				olc::Pixel(0x40ui8, 0x40ui8, 0x40ui8)
#define COLOR_BLACK					olc::Pixel(0x00ui8, 0x00ui8, 0x00ui8)

#define COLOR_LIGHT_RED				olc::Pixel(0xF8ui8, 0x38ui8, 0x00ui8)
#define COLOR_RED					olc::Pixel(0xBCui8, 0x10ui8, 0x00ui8)
#define COLOR_DARK_RED				olc::Pixel(0x58ui8, 0x00ui8, 0x00ui8)

#define COLOR_VERY_LIGHT_GREEN		olc::Pixel(0xB8ui8, 0xF8ui8, 0xB8ui8)
#define COLOR_LIGHT_GREEN			olc::Pixel(0x58ui8, 0xF8ui8, 0x54ui8)
#define COLOR_GREEN					olc::Pixel(0x00ui8, 0xA8ui8, 0x00ui8)
#define COLOR_DARK_GREEN			olc::Pixel(0x00ui8, 0x80ui8, 0x00ui8)
#define COLOR_VERY_DARK_GREEN		olc::Pixel(0x00ui8, 0x58ui8, 0x00ui8)

#define COLOR_VERY_LIGHT_BLUE		olc::Pixel(0xA8ui8, 0xE4ui8, 0xF8ui8)
#define COLOR_LIGHT_BLUE			olc::Pixel(0x3Cui8, 0xBCui8, 0xF8ui8)
#define COLOR_BLUE					olc::Pixel(0x00ui8, 0x78ui8, 0xF8ui8)
#define COLOR_DARK_BLUE				olc::Pixel(0x00ui8, 0x40ui8, 0xF8ui8)
#define COLOR_VERY_DARK_BLUE		olc::Pixel(0x00ui8, 0x20ui8, 0x9Cui8)

#define COLOR_LIGHT_YELLOW			olc::Pixel(0xF8ui8, 0xE0ui8, 0xA8ui8)
#define COLOR_YELLOW				olc::Pixel(0xF8ui8, 0xBCui8, 0x00ui8)
#define COLOR_DARK_YELLOW			olc::Pixel(0xACui8, 0x7Cui8, 0x00ui8)
#define COLOR_VERY_DARK_YELLOW		olc::Pixel(0x50ui8, 0x30ui8, 0x00ui8)

#define COLOR_VERY_LIGHT_ORANGE		olc::Pixel(0xF8ui8, 0xD0ui8, 0xA8ui8)
#define COLOR_LIGHT_ORANGE			olc::Pixel(0xF8ui8, 0xA0ui8, 0x44ui8)
#define COLOR_ORANGE				olc::Pixel(0xE4ui8, 0xBCui8, 0x10ui8)
#define COLOR_DARK_ORANGE			olc::Pixel(0x88ui8, 0x30ui8, 0x00ui8)

#define COLOR_VERY_LIGHT_PURPLE		olc::Pixel(0xD8ui8, 0xB8ui8, 0xF8ui8)
#define COLOR_LIGHT_PURPLE			olc::Pixel(0x98ui8, 0x78ui8, 0xF8ui8)
#define COLOR_PURPLE				olc::Pixel(0x68ui8, 0x44ui8, 0xFCui8)
#define COLOR_DARK_PURPLE			olc::Pixel(0x44ui8, 0x28ui8, 0xBCui8)

#define COLOR_VERY_LIGHT_PINK		olc::Pixel(0xF8ui8, 0xCCui8, 0xE4ui8)
#define COLOR_LIGHT_PINK			olc::Pixel(0xF8ui8, 0x7Cui8, 0xE4ui8)
#define COLOR_PINK					olc::Pixel(0xF8ui8, 0x00ui8, 0xE4ui8)
#define COLOR_DARK_PINK				olc::Pixel(0xBCui8, 0x00ui8, 0xA4ui8)
#define COLOR_VERY_DARK_PINK		olc::Pixel(0x7Cui8, 0x00ui8, 0x6Eui8)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	A trio of macros that determines the dimensions of the viewport into the game's world, as well as the starting width and height of	//
//	the program's window when multiplied by the WINDOW_SCALE macro that is below said width and height values.							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define VIEWPORT_WIDTH					640
#define VIEWPORT_HEIGHT					360
#define WINDOW_SCALE					2

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Condenses the code required for initializing a singleton (Within the .hpp file of said singleton class) into a single-line define.	//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define INIT_SINGLETON_HPP(_class)							\
			protected:										\
				static _class* s_instance;					\
			public:											\
				_class(_class &_other) = delete;			\
				void operator=(const _class&) = delete;		\
				static _class* GetInstance();

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Condenses the code required for initializing a singleton (Within the .cpp file of said singleton class) into a single-line define.	//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define INIT_SINGLETON_CPP(_class)							\
			_class* _class::s_instance = nullptr;			\
			_class* _class::GetInstance(){					\
				if (s_instance == nullptr)					\
					s_instance = new _class();				\
				return s_instance;							\
			}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A simple define that condenses the typing required to reference a singleton class within the code.									//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define	GET_SINGLETON(_class)			_class::GetInstance()

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that are important for the state machine systems that can be utilized within various classes.								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define INVALID_STATE					0xFFui8
#define SET_NEXT_STATE(_state)			SetNextState(_state, nextState)
#define UPDATE_STATE(_state)			UpdateCurrentState(_state, curState, lastState)

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Macros that represent the section headers for the settings saved within "settings.ini". Basically prevents accidental typos when	//
//	referencing each section since the macro auotfills itself but a string won't care if it matches another when hardcoded.				//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define SETTINGS_GROUP_VIDEO			"[VIDEO]"
#define SETTINGS_GROUP_AUDIO			"[AUDIO]"
#define SETTINGS_GROUP_INPUT			"[INPUT]"
#define SETTINGS_GROUP_ACCESSIBILITY	"[ACCESSIBILITY]"

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Macros that represent the key values for each respective group's volume value, which is a floating point number ranging between		//
//  0.0 and 1.0. The master volume will further reduce the volumes of both the music and sound effect volume levels.					//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define AUDIO_MASTER_VOLUME				"Master"
#define AUDIO_MUSIC_VOLUME				"Music"
#define AUDIO_SOUND_EFFECT_VOLUME		"Sounds"

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Macros that represent the key values for their respective input bindings within the master input bindings map stored in the game's	//
//	settings singleton class.																											//
// ------------------------------------------------------------------------------------------------------------------------------------ //

// ------- Inputs that are unique to in-game actions -------- //

#define INPUT_MOVE_RIGHT				"MoveRight"
#define INPUT_MOVE_LEFT					"MoveLeft"
#define INPUT_MOVE_UP					"MoveUp"
#define INPUT_MOVE_DOWN					"MoveDown"
#define INPUT_INTERACT					"Interact"

// ---------------------------------------------------------- //

// --- Inputs that are unique to any menu within the game --- //

#define INPUT_MENU_RIGHT				"MenuRight"
#define INPUT_MENU_LEFT					"MenuLeft"
#define INPUT_MENU_UP					"MenuUp"
#define INPUT_MENU_DOWN					"MenuDown"
#define INPUT_MENU_SELECT				"MenuSelect"
#define INPUT_MENU_RETURN				"MenuReturn"
#define INPUT_SAVE_FILE_DELETE			"SaveFileDelete"
#define INPUT_SAVE_FILE_COPY			"SaveFileCopy"

// ---------------------------------------------------------- //

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define PARTY_ROSTER_MAX_SIZE			8ui64
#define PARTY_ACTIVE_MAX_SIZE			3ui64

#endif