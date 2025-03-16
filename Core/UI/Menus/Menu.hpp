#ifndef MENU_HPP
#define MENU_HPP

#include "../../Singletons/EngineCore.hpp"
#include "../../Utils/MenuMacros.hpp"

struct MenuOption;

class Menu {
public: // Constructor and Destructor Declarations
	Menu();
	~Menu() = default;

public: // Main Engine Function Declarations
	virtual bool OnUserCreate() = 0;
	bool OnUserDestroy();
	virtual bool OnUserUpdate(float_t _deltaTime) = 0;
	virtual bool OnUserRender(float_t _deltaTime) = 0;

	bool OnBeforeUserUpdate(float_t _deltaTime);
	void OnAfterUserUpdate(float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	void AddOption(int32_t _xPos, int32_t _yPos, const std::string& _mainText, const std::string& _description = "", uint8_t _alpha = 0xFFui8,
		uint32_t _flags = FLAG_MENU_OPTION_ACTIVE_STATE | FLAG_MENU_OPTION_SELECTABLE | FLAG_MENU_OPTION_VISIBLE);
	void RemoveOption(size_t _index);
	void SetOptionFlags(size_t _index, uint32_t _flags, bool _overwrite = true);

	// 
	inline void SetFlags(uint32_t _flags, bool _overwrite = true) {
		if (_overwrite) {
			flags = _flags;
			return;
		}
		flags |= _flags;
	}

	// 
	inline uint32_t GetFlags() const {
		return flags;
	}
protected: // Hidden (Accessible to Children Only) Utility Function Declarations
	void InitializeParams(uint8_t _state, uint8_t _width, uint8_t _visibleRows, uint8_t _visibleColumns, uint8_t _rowShiftOffset = 0ui8, uint8_t _columnShiftOffset = 0ui8,
		uint8_t _alpha = 0xFFui8, uint32_t _flags = FLAG_MENU_ACTIVE_STATE | FLAG_MENU_VISIBLE);
	void InitializeOptionParams(int32_t _anchorX, int32_t _anchorY, int32_t _spacingX, int32_t _spacingY, olc::Pixel _color = COLOR_WHITE, olc::Pixel _hoverColor = COLOR_LIGHT_YELLOW,
		olc::Pixel _selColor = COLOR_LIGHT_GREEN);
	void InitializeDescriptionParams(int32_t _x, int32_t _y);

	void UpdateCursor(float_t _deltaTime);
	void RenderVisibleOptions(float_t _deltaTime);

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

	olc::Pixel optionColor;			// Each stores the color of a menu option depending on if it's visible, below the cursor, or selected.
	olc::Pixel optionHoverColor;
	olc::Pixel optionSelColor;
};

struct MenuTitle {
	int32_t		xPos;
	int32_t		yPos;
	std::string text;
};

struct MenuOption {
	int32_t		xPos;
	int32_t		yPos;

	std::string	text;
	std::string	description;

	uint32_t	flags;
	uint8_t		alpha;

	// Renders the menu to the screen at the relevant coordinates. The engine pointer is required to avoid having to use 
	// GET_SINGLETON for each individual menu option that is visible. That value is cached before the rendering loop and
	// simply passed along to each option during its rendering.
	inline void DrawSelf(EngineCore* _engine, int32_t _x, int32_t _y, olc::Pixel _color = olc::WHITE, uint8_t _alpha = 0xFFui8) const {
		if (_alpha == 0ui8 || alpha == 0ui8)
			return;

		_color.a = uint16_t(_alpha + alpha) / 2ui8;
		_engine->DrawString(_x + xPos, _y + yPos, text, _color);
	}
};

#endif