#ifndef MENU_HPP
#define MENU_HPP

#include "../../singleton/EngineCore.hpp"
#include "../../struct/user_interface/menu/MenuOption.hpp"
#include "../../utility/MenuMacros.hpp"
#include "../../utility/GeneralMacros.hpp"
#include "../../utility/UtilityFunctions.hpp"

#include <math.h>
#include <cstdint>
#include <string>
#include <vector>

struct MenuOption;

class Menu {
public: // Constructor and Destructor Declarations
	Menu();
	~Menu() = default;

public: // Main Engine Function Declarations
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserDestroy();
	virtual bool OnUserUpdate(float_t _deltaTime);
	virtual bool OnUserRender(EngineCore* _engine, float_t _deltaTime);

	bool OnBeforeUserUpdate(float_t _deltaTime);
	bool OnAfterUserUpdate(float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	void AddOption(int32_t _xPos, int32_t _yPos, const std::string& _mainText, const std::string& _description = "", uint8_t _alpha = 0xFFui8,
		uint32_t _flags = FLAG_MOPTION_ACTIVE_STATE | FLAG_MOPTION_SELECTABLE | FLAG_MOPTION_VISIBLE);
	void RemoveOption(size_t _index);
	void SetOptionFlags(size_t _index, uint32_t _flags, bool _overwrite = true);

	// An extension of the standard "SetNextState" function that is utilized by all classes containing a state machine. On top 
	// of updating the current state as required, it can also free the menu from being locked due to the selOption value being
	// set to a valid menu option index.
	inline void MenuSetNextState(uint8_t _state, bool _resetSelOption = false) {
		SET_NEXT_STATE(_state);
		if (_resetSelOption) // Optional addition that reset the selected option for the menu to the default.
			selOption = MENU_SELECTION_INVALID;
	}

	// Allows other menus/obejcts to manipulate the menu's current flags. Useful when passing control between menus.
	inline void SetFlags(int32_t _flags) {
		flags &= _flags;
	}

	// Simply returns the current flags that are enabled or disabled for the menu.
	inline uint32_t GetFlags() const {
		return flags;
	}

	// Similar to the function above, but return the menu's current input flags instead of its general flags. It doesn't take 
	// into account if the input has been pressed or not; only if it's currently held or not held by the user.
	inline uint32_t GetActiveInputs() const {
		return inputFlags;
	}

	// ANother input flag function, but for only when the inputs have been pressed instead of pressed or held by the user.
	inline uint32_t GetPressedInputs() const {
		return inputFlags ^ prevInputFlags;
	}
protected: // Hidden (Accessible to Children Only) Utility Function Declarations
	void InitializeParams(uint8_t _state, uint8_t _width, uint8_t _visibleRows, uint8_t _visibleColumns, uint8_t _rowShiftOffset = 0ui8, uint8_t _columnShiftOffset = 0ui8,
		uint8_t _alpha = 0xFFui8, uint32_t _flags = FLAG_MENU_ACTIVE_STATE | FLAG_MENU_VISIBLE);
	void InitializeOptionParams(int32_t _anchorX, int32_t _anchorY, int32_t _spacingX, int32_t _spacingY, olc::Pixel _color = COLOR_WHITE, olc::Pixel _hoverColor = COLOR_LIGHT_YELLOW,
		olc::Pixel _selColor = COLOR_LIGHT_GREEN, olc::Pixel _inactiveColor = COLOR_GRAY);
	void InitializeDescriptionParams(int32_t _x, int32_t _y);

	void UpdateCursor(float_t _deltaTime);
	void RenderVisibleOptions(EngineCore* _engine, float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	void PrepareForActivation(uint8_t _state);
	void PrepareForDeactivation();

protected: // Hidden (Accessible to Children Only) State Function Declarations
	virtual bool StateDefault(float_t _deltaTime);
	virtual bool StateProcessSelection() = 0;

protected: // Hidden (Accessible to Children Only) Member Variable Declarations
	int32_t xPos;
	int32_t yPos;

	Menu* upperMenu;
	Menu* subMenu;

	std::string auxSelect;
	std::string auxReturn;

	uint32_t flags;
	uint32_t inputFlags;
	uint32_t prevInputFlags;

	uint8_t	curState;
	uint8_t nextState;
	uint8_t lastState;

	std::vector<MenuOption> menuOptions;

	uint8_t alpha;					// Controls the current visibility of the entire menu.

	uint8_t curOption;
	uint8_t selOption;
	uint8_t auxSelOption;

	float_t cursorMoveTimer;

	uint8_t numVisibleRows;			// Sets how many options are visible along the x and y axis. The menu's width takes precendent if this value is too high.
	uint8_t numVisibleColumns;
	uint8_t curVisibleRowOffset;	// Determines the offset relative to 0 the the visible region is at within the full array of options.
	uint8_t curVisibleColumnOffset;

	uint8_t rowShiftOffset;			// Determines how far the menu cursor needs to be from the edges of the visible region of options before they get shifted.
	uint8_t columnShiftOffset;

	uint8_t menuWidth;				// The dimensions of the menu (Width is manually set; height is dynamically calculated relative to number of options).
	uint8_t menuHeight;

	int32_t optionAnchorX;			// The coordinates to position the top-most/left-most visible option.
	int32_t optionAnchorY;
	
	int32_t optionDescriptionX;		// The position of the highlighted/selected option's description on the screen.
	int32_t optionDescriptionY;

	int32_t optionSpacingX;			// Determines the gap (Excluding the dimensions of the option's contents itself) between options along the x and y axis.
	int32_t optionSpacingY;

	olc::Pixel optionColor;			// Each stores the color of a menu option depending on if it's visible, below the cursor, selected, or inactive.
	olc::Pixel optionHoverColor;
	olc::Pixel optionSelColor;
	olc::Pixel optionInactiveColor;
};

#endif