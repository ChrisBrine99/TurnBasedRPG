#ifndef GENERAL_MACROS_HPP
#define GENERAL_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for colors that will be utilized all over the code's GUI elements.															//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define COLOR_TRUE_WHITE				olc::Pixel(0xFFFFFFFFui32)	// RGBA 255, 255, 255, 255
#define COLOR_WHITE						olc::Pixel(0xFFF8F8F8ui32)	// RGBA 248, 248, 248, 255
#define COLOR_LIGHT_GRAY				olc::Pixel(0xFFBCBCBCui32)	// RGBA 188, 188, 188, 255
#define COLOR_GRAY						olc::Pixel(0xFF7C7C7Cui32)	// RGBA 124, 124, 124, 255
#define COLOR_DARK_GRAY					olc::Pixel(0xFF404040ui32)	// RGBA  64,  64,  64, 255
#define COLOR_BLACK						olc::Pixel(0xFF000000ui32)	// RGBA   0,   0,   0, 255

#define COLOR_VERY_LIGHT_RED			olc::Pixel(0xFFBCBCF8ui32)	// RGBA 248, 188, 188, 255
#define COLOR_LIGHT_RED					olc::Pixel(0xFF3050F8ui32)	// RGBA 248,  80,  48, 255
#define	COLOR_RED						olc::Pixel(0xFF0010E0ui32)	// RGBA 224,  16,   0, 255
#define COLOR_DARK_RED					olc::Pixel(0xFF0010BCui32)	// RGBA 168,  16,   0, 255
#define COLOR_VERY_DARK_RED				olc::Pixel(0xFF000058ui32)	// RGBA  88,   0,   0, 255

#define COLOR_VERY_LIGHT_LIME			olc::Pixel(0xFFBCF8E0ui32)	// RGBA 224, 248, 188, 255
#define COLOR_LIGHT_LIME				olc::Pixel(0xFF94F8BCui32)	// RGBA 188, 248, 148, 255
#define COLOR_LIME						olc::Pixel(0xFF58F890ui32)	// RGBA 144, 248,  88, 255
#define COLOR_DARK_LIME					olc::Pixel(0xFF30F868ui32)	// RGBA 104, 248,  48, 255
#define COLOR_VERY_DARK_LIME			olc::Pixel(0xFF00E040ui32)	// RGBA  64, 224,  64, 255

#define COLOR_VERY_LIGHT_GREEN			olc::Pixel(0xFF7CF87Cui32)	// RGBA 128, 248, 128, 255
#define COLOR_LIGHT_GREEN				olc::Pixel(0xFF00F800ui32)	// RGBA   0, 248,   0, 255
#define COLOR_GREEN						olc::Pixel(0xFF00BC00ui32)	// RGBA   0, 188,   0, 255
#define COLOR_DARK_GREEN				olc::Pixel(0xFF008C00ui32)	// RGBA   0, 140,   0, 255
#define COLOR_VERY_DARK_GREEN			olc::Pixel(0xFF006400ui32)	// RGBA	  0, 100,   0, 255

#define COLOR_VERY_LIGHT_CYAN			olc::Pixel(0xFFF8F87Cui32)	// RGBA 124, 248, 248, 255
#define COLOR_LIGHT_CYAN				olc::Pixel(0xFFF8F840ui32)	// RGBA  48, 248, 248, 255
#define COLOR_CYAN						olc::Pixel(0xFFCCCC00ui32)	// RGBA   0, 204, 204, 255
#define COLOR_DARK_CYAN					olc::Pixel(0xFFA0A000ui32)	// RGBA   0, 160, 160, 255
#define COLOR_VERY_DARK_CYAN			olc::Pixel(0xFF7C7C00ui32)	// RGBA   0, 127, 127, 255

#define COLOR_VERY_LIGHT_BLUE			olc::Pixel(0xFFF8A480ui32)	// RGBA	128, 164, 248, 255
#define COLOR_LIGHT_BLUE				olc::Pixel(0xFFF87C58ui32)	// RGBA  88, 128, 248, 255
#define COLOR_BLUE						olc::Pixel(0xFFF84020ui32)	// RGBA  32,  64, 248, 255
#define COLOR_DARK_BLUE					olc::Pixel(0xFFBC2010ui32)	// RGBA  16,  32, 188, 255
#define COLOR_VERY_DARK_BLUE			olc::Pixel(0xFF7C0000ui32)	// RGBA   0,   0, 124, 255

#define COLOR_VERY_LIGHT_YELLOW			olc::Pixel(0xFFA8F8F8ui32)	// RGBA 248, 248, 168, 255
#define COLOR_LIGHT_YELLOW				olc::Pixel(0xFF7CF8F8ui32)	// RGBA 248, 248, 124, 255
#define COLOR_YELLOW					olc::Pixel(0xFF00F8F8ui32)	// RGBA 236, 236,   0, 255
#define COLOR_DARK_YELLOW				olc::Pixel(0xFF00ACACui32)	// RGBA 172, 172,   0, 255
#define COLOR_VERY_DARK_YELLOW			olc::Pixel(0xFF005050ui32)	// RGBA  80,  80,   0, 255

#define COLOR_VERY_LIGHT_ORANGE			olc::Pixel(0xFFA8D0F8ui32)	// RGBA 248, 208, 168, 255
#define COLOR_LIGHT_ORANGE				olc::Pixel(0xFF44BCF8ui32)	// RGBA 248, 188,  68, 255
#define COLOR_ORANGE					olc::Pixel(0xFF10A0E4ui32)	// RGBA 228, 160,  16, 255
#define COLOR_DARK_ORANGE				olc::Pixel(0xFF0050BCui32)	// RGBA 188,  80,   0, 255
#define COLOR_VERY_DARK_ORANGE			olc::Pixel(0xFF003088ui32)	// RGBA 136,  48,   0, 255

#define COLOR_VERY_LIGHT_PURPLE			olc::Pixel(0xFFF8B8D8ui32)	// RGBA 216, 184, 248, 255
#define COLOR_LIGHT_PURPLE				olc::Pixel(0xFFF894B8ui32)	// RGBA 184, 148, 248, 255
#define COLOR_PURPLE					olc::Pixel(0xFFF87898ui32)	// RGBA 152, 120, 248, 255
#define COLOR_DARK_PURPLE				olc::Pixel(0xFFF84468ui32)	// RGBA 104,  68, 248, 255
#define COLOR_VERY_DARK_PURPLE			olc::Pixel(0xFFBC2844ui32)	// RGBA  68,  40, 188, 255

#define COLOR_VERY_LIGHT_PINK			olc::Pixel(0xFFE4CCF8ui32)	// RGBA 248, 204, 228, 255
#define COLOR_LIGHT_PINK				olc::Pixel(0xFFE47CF8ui32)	// RGBA 248, 124, 228, 255
#define COLOR_PINK						olc::Pixel(0xFFE400F8ui32)	// RGBA 248,   0, 228, 255
#define COLOR_DARK_PINK					olc::Pixel(0xFFA400BCui32)	// RGBA 188,   0, 164, 255
#define COLOR_VERY_DARK_PINK			olc::Pixel(0xFF6E007Cui32)	// RGBA 124,   0, 110, 255

#define COLOR_VERY_LIGHT_BROWN			olc::Pixel(0xFF0094CCui32)	// RGBA 204, 148,   0, 255
#define COLOR_LIGHT_BROWN				olc::Pixel(0xFF006CA4ui32)	// RGBA 164, 108,   0, 255
#define COLOR_BROWN						olc::Pixel(0xFF00588Aui32)	// RGBA 138,  88,   0, 255
#define COLOR_DARK_BROWN				olc::Pixel(0xFF003058ui32)	// RGBA  88,  48,   0, 255
#define COLOR_VERY_DARK_BROWN			olc::Pixel(0xFF001830ui32)	// RGBA  48,  24,   0, 255

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Simply determines how fast dynamic objects will move relative to their set speed, this value, and the current delta time.			//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define TARGET_FPS						60.0f

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	A trio of macros that determines the dimensions of the viewport into the game's world, as well as the starting width and height of	//
//	the program's window when multiplied by the WINDOW_SCALE macro that is below said width and height values.							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define VIEWPORT_WIDTH					640i32
#define VIEWPORT_HEIGHT					360i32
#define WINDOW_SCALE					2i32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Floating point variants of the three macros above.																					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define VIEWPORT_WIDTH_F				640.0f
#define VIEWPORT_HEIGHT_F				360.0f
#define WINDOW_SCALE_F					2.0f

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	A value that is equivalent to roughly 1/60th of a second. Allows fixed updating for code that requires it.							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define GAME_UPDATE_INTERVAL			0.0167f

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Condenses the code required for initializing a singleton (Within the .hpp file of said singleton class) into a single-line define.	//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define INIT_SINGLETON_HPP(_x)						\
			private:								\
				_x();								\
				~_x() = default;					\
			public:									\
				_x(const _x&) = delete;				\
				void operator=(const _x&) = delete;	\
			protected:								\
				static _x* s_instance;				\
			public:									\
				static _x* GetInstance();			\

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Condenses the code required for initializing a singleton (Within the .cpp file of said singleton class) into a single-line define.	//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define INIT_SINGLETON_CPP(_x)				\
			_x* _x::s_instance = nullptr;	\
			_x* _x::GetInstance(){			\
				if (s_instance == nullptr)	\
					s_instance = new _x();	\
				return s_instance;			\
			}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that reduce the clutter required to execute various singleton class create/update functions. They will automatically		//
//	return out of the upper function as false if any issues occurred during their execution. The functions themselves aren't provided	//
//	since both create and update have a pre- and post- event that can be called if required before and after the main create/update		//
//  funciton, respectively.																												//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define CALL_SINGLETON_CREATE(_x, _func)		\
			if (!GET_SINGLETON(_x)->_func())	\
				return false

#define CALL_SINGLETON_UPDATE(_x, _func)		\
			if (!GET_SINGLETON(_x)->_func())	\
				return false

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that work like the "CALL_SINGLETON_CREATE" and "CALL_SINGLETON_UPDATE" defines detailed above. However, they don't take in	//
//	a function as the second parameter since there is only ever one destroy and one render function in the engine.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define CALL_SINGLETON_DESTROY(_x)							\
			if (!GET_SINGLETON(_x)->OnUserDestroy())		\
				return false

#define CALL_SINGLETON_RENDER(_x)							\
			if (!GET_SINGLETON(_x)->OnUserRender(_engine))	\
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