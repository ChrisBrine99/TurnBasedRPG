#ifndef GENERAL_MACROS_HPP
#define GENERAL_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for colors that will be utilized all over the code's GUI elements.															//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define COLOR_WHITE					olc::Pixel(0xFFF8F8F8ui32)	// RGBA 248, 248, 248, 255
#define COLOR_LIGHT_GRAY			olc::Pixel(0xFFBCBCBCui32)	// RGBA 188, 188, 188, 255
#define COLOR_GRAY					olc::Pixel(0xFF7C7C7Cui32)	// RGBA 124, 124, 124, 255
#define COLOR_DARK_GRAY				olc::Pixel(0xFF404040ui32)	// RGBA  64,  64,  64, 255
#define COLOR_BLACK					olc::Pixel(0xFF000000ui32)	// RGBA   0,   0,   0, 255

#define COLOR_LIGHT_RED				olc::Pixel(0xFF0032F8ui32)	// RGBA 248,  50,   0, 255
#define COLOR_RED					olc::Pixel(0xFF0010BCui32)	// RGBA 188,  16,   0, 255
#define COLOR_DARK_RED				olc::Pixel(0xFF000058ui32)	// RGBA  88,   0,   0, 255

#define COLOR_VERY_LIGHT_GREEN		olc::Pixel(0xFFB8F8B8ui32)	// RGBA 184, 248, 184, 255
#define COLOR_LIGHT_GREEN			olc::Pixel(0xFF54F854ui32)	// RGBA  84, 248,  84, 255
#define COLOR_GREEN					olc::Pixel(0xFF00A800ui32)	// RGBA   0, 168,   0, 255
#define COLOR_DARK_GREEN			olc::Pixel(0xFF008000ui32)	// RGBA   0, 128,   0, 255
#define COLOR_VERY_DARK_GREEN		olc::Pixel(0xFF005800ui32)	// RGBA	  0,  88,   0, 255

#define COLOR_VERY_LIGHT_BLUE		olc::Pixel(0xFFF8E4A8ui32)	// RGBA 168, 228, 248, 255
#define COLOR_LIGHT_BLUE			olc::Pixel(0xFFF8BC3Cui32)	// RGBA  60, 188, 248, 255
#define COLOR_BLUE					olc::Pixel(0xFFF87800ui32)	// RGBA   0, 120, 248, 255
#define COLOR_DARK_BLUE				olc::Pixel(0xFFF84000ui32)	// RGBA   0,  64, 248, 255
#define COLOR_VERY_DARK_BLUE		olc::Pixel(0xFF9C2000ui32)	// RGBA   0,  32, 156, 255

#define COLOR_LIGHT_YELLOW			olc::Pixel(0xFFA8E0F8ui32)	// RGBA 248, 224, 168, 255
#define COLOR_YELLOW				olc::Pixel(0xFF00BCF8ui32)	// RGBA 248, 188,   0, 255
#define COLOR_DARK_YELLOW			olc::Pixel(0xFF007CACui32)	// RGBA 172, 124,   0, 255
#define COLOR_VERY_DARK_YELLOW		olc::Pixel(0xFF003050ui32)	// RGBA  80,  48,   0, 255

#define COLOR_VERY_LIGHT_ORANGE		olc::Pixel(0xFFA8D0F8ui32)	// RGBA 248, 208, 168, 255
#define COLOR_LIGHT_ORANGE			olc::Pixel(0xFF44BCF8ui32)	// RGBA 248, 188,  68, 255
#define COLOR_ORANGE				olc::Pixel(0xFF10A0E4ui32)	// RGBA 228, 160,  16, 255
#define COLOR_DARK_ORANGE			olc::Pixel(0xFF003088ui32)	// RGBA 136,  48,   0, 255

#define COLOR_VERY_LIGHT_PURPLE		olc::Pixel(0xFFF8B8D8ui32)	// RGBA 216, 184, 248, 255
#define COLOR_LIGHT_PURPLE			olc::Pixel(0xFFF87898ui32)	// RGBA 152, 120, 248, 255
#define COLOR_PURPLE				olc::Pixel(0xFFF84468ui32)	// RGBA 104,  68, 248, 255
#define COLOR_DARK_PURPLE			olc::Pixel(0xFFBC2844ui32)	// RGBA  68,  40, 188, 255

#define COLOR_VERY_LIGHT_PINK		olc::Pixel(0xFFE4CCF8ui32)	// RGBA 248, 204, 228, 255
#define COLOR_LIGHT_PINK			olc::Pixel(0xFFE47CF8ui32)	// RGBA 248, 124, 228, 255
#define COLOR_PINK					olc::Pixel(0xFFE400F8ui32)	// RGBA 248,   0, 228, 255
#define COLOR_DARK_PINK				olc::Pixel(0xFFA400BCui32)	// RGBA 188,   0, 164, 255
#define COLOR_VERY_DARK_PINK		olc::Pixel(0xFF6E007Cui32)	// RGBA 124,   0, 110, 255

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	A trio of macros that determines the dimensions of the viewport into the game's world, as well as the starting width and height of	//
//	the program's window when multiplied by the WINDOW_SCALE macro that is below said width and height values.							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define VIEWPORT_WIDTH					640
#define VIEWPORT_HEIGHT					360
#define WINDOW_SCALE					2

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	A value that is equivalent to roughly 1/60th of a second. Allows fixed updating for code that requires it.							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define GAME_UPDATE_INTERVAL			0.0167f

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Condenses the code required for initializing a singleton (Within the .hpp file of said singleton class) into a single-line define.	//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define INIT_SINGLETON_HPP(_x)									\
			private:											\
				_x();											\
				~_x() = default;								\
			public:												\
				_x(const _x&) = delete;							\
				void operator=(const _x&) = delete;				\
			protected:											\
				static _x* s_instance;							\
			public:												\
				static _x* GetInstance();						\

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Condenses the code required for initializing a singleton (Within the .cpp file of said singleton class) into a single-line define.	//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define INIT_SINGLETON_CPP(_x)									\
			_x* _x::s_instance = nullptr;						\
			_x* _x::GetInstance(){								\
				if (s_instance == nullptr)						\
					s_instance = new _x();						\
				return s_instance;								\
			}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that reduce the clutter required to execute various singleton class create/update functions. They will automatically		//
//	return out of the upper function as false if any issues occurred during their execution. The functions themselves aren't provided	//
//	since both create and update have a pre- and post- event that can be called if required before and after the main create/update		//
//  funciton, respectively.																												//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define CALL_SINGLETON_CREATE(_x, _func)						\
			if (!GET_SINGLETON(_x)->_func())					\
				return false

#define CALL_SINGLETON_UPDATE(_x, _func)						\
			if (!GET_SINGLETON(_x)->_func(_deltaTime))			\
				return false

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that work like the "CALL_SINGLETON_CREATE" and "CALL_SINGLETON_UPDATE" defines detailed above. However, they don't take in	//
//	a function as the second parameter since there is only ever one destroy and one render function in the engine.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define CALL_SINGLETON_DESTROY(_x)								\
			if (!GET_SINGLETON(_x)->OnUserDestroy())			\
				return false

#define CALL_SINGLETON_RENDER(_x)								\
			if (!GET_SINGLETON(_x)->OnUserRender(_deltaTime))	\
				return false

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A simple define that condenses the typing required to reference a singleton class within the code.									//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define	GET_SINGLETON(_x)				_x::GetInstance()

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A define that sets the value 255 (Maximum possible value that can be stored in a uint8_t) as the known invalid index for states.	//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define STATE_INVALID					0xFFui8

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
//	Defines that will determine how many party members the player can have in their roster at once, as well as the amount of those		//
//	that can set to the active roster versus the reserve roster.																		//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define PARTY_ROSTER_MAX_SIZE			5ui64
#define PARTY_ACTIVE_MAX_SIZE			3ui64

#endif