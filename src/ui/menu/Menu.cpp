#include "Menu.hpp"

#include "../../singleton/GameSettings.hpp"
#include "../../singleton/MenuManager.hpp"

Menu::Menu() :
	xPos(0.0f),
	yPos(0.0f),
	upperMenu(nullptr),
	subMenu(nullptr),
	auxSelect(""),
	auxReturn(""),
	flags(0ui32),
	inputFlags(0ui32),
	prevInputFlags(0ui32),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID),
	menuOptions(),
	curOption(0ui8),
	selOption(MENU_SELECTION_INVALID),
	auxSelOption(MENU_SELECTION_INVALID),
	cursorMoveTimer(0.0f),
	numVisibleRows(0ui8),
	numVisibleColumns(0ui8),
	curVisibleRowOffset(0ui8),
	curVisibleColumnOffset(0ui8),
	rowShiftOffset(0ui8),
	columnShiftOffset(0ui8),
	menuWidth(0ui8),
	menuHeight(0ui8),
	optionAnchorX(0.0f),
	optionAnchorY(0.0f),
	optionDescriptionX(0.0f),
	optionDescriptionY(0.0f),
	optionSpacingX(0.0f),
	optionSpacingY(0.0f),
	alpha(0ui8),
	optionColor(COLOR_WHITE),
	optionHoverColor(COLOR_WHITE),
	optionSelColor(COLOR_WHITE),
	optionInactiveColor(COLOR_WHITE)
{ // Reserve a small portion of memory to store the pointers for the menu options that can exist within a given menu.
	menuOptions.reserve(10ui64);
}

bool Menu::OnUserDestroy() {
	menuOptions.clear();
	menuOptions.shrink_to_fit();
	return true;
}

bool Menu::OnUserUpdate() {
	switch (curState) {
	case STATE_MENU_DEFAULT:			return StateDefault();
	case STATE_MENU_PROCESS_SELECTION:	return StateProcessSelection();
	case STATE_INVALID:					return true;
	}
	
	return false;
}

bool Menu::OnUserRender(EngineCore* _engine) {
	RenderVisibleOptions(_engine);
	return true;
}

bool Menu::OnBeforeUserUpdate() {
	if (MENU_IS_BLOCKING_INPUT)
		return true;
	prevInputFlags = inputFlags;

	GameSettings* _settings = GET_SINGLETON(GameSettings);
	inputFlags =	
		(uint32_t(_settings->GetKeyInfo(INPUT_MENU_RIGHT).bHeld)			  ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_MENU_LEFT).bHeld)			<< 1u ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_MENU_UP).bHeld)			<< 2u ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_MENU_DOWN).bHeld)			<< 3u ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_MENU_SELECT).bReleased)	<< 4u ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_MENU_RETURN).bReleased)	<< 5u );

	if (auxSelect != "") { // Only check for the auxiliary select button if one has been set by the menu.
		inputFlags |= (uint32_t(_settings->GetKeyInfo(auxSelect.c_str()).bPressed) << 6u);
	}

	if (auxReturn != "") { // Only check for the auxiliary return button if one has been set by the menu.
		inputFlags |= (uint32_t(_settings->GetKeyInfo(auxReturn.c_str()).bPressed) << 7u);
	}

	return true;
}

bool Menu::OnAfterUserUpdate() {
	UPDATE_STATE(nextState);
	return true;
}

void Menu::AddOption(float_t _xPos, float_t _yPos, const std::string& _mainText, const std::string& _description, uint8_t _alpha, uint32_t _flags) {
	if (!MENU_ARE_OPTIONS_ALLOWED) // Prevent menus from adding options before they've been fully initialized.
		return;

	MenuOption _newOption{
		_xPos,		_yPos,			// Top-left coordinates of the menu option on the screen.
		_mainText,	_description,	// Text that represents the option for the user, and the optional description text that explains selecting it does.
		_flags,		_alpha
	};

	menuOptions.push_back(_newOption);
	menuHeight = 1ui8 + uint8_t(menuOptions.size()) / menuWidth;
}

void Menu::RemoveOption(size_t _index) {
	if (_index >= menuOptions.size() || !MENU_IS_ACTIVE)
		return;
	menuOptions.erase(menuOptions.begin() + _index);
	menuHeight = 1ui8 + uint8_t(menuOptions.size()) / menuWidth;
}

void Menu::SetOptionFlags(size_t _index, uint32_t _flags, bool _overwrite) {
	if (_index >= menuOptions.size())
		return;

	if (_overwrite) { menuOptions[_index].flags  = _flags; }
	else			{ menuOptions[_index].flags |= _flags; }
}

void Menu::InitializeParams(uint8_t _state, uint8_t _width, uint8_t _visibleRows, uint8_t _visibleColumns, uint8_t _rowShiftOffset, uint8_t _columnShiftOffset, uint8_t _alpha, uint32_t _flags) {
	if (MENU_IS_INITIALIZED)
		return; // Don't allow initialization of a menu twice.
	flags = _flags | FLAG_MENU_INITIALIZED;
	SET_NEXT_STATE(_state);

	// Assign the menu's height, but prevent it from being 0.
	menuWidth = std::max(_width, 1ui8);

	// Assign the menu's total number of visible options along the x and y axes. Much like the menu's width, these values cannot 
	// be set to 0.
	numVisibleRows = std::max(_visibleRows, 1ui8);
	numVisibleColumns = std::max(_visibleColumns, 1ui8);

	// Assign the buffer region between the cursor and edge of the visible portion of the menu before the options get shifted by one 
	// row/column relative to the direction the cursor happens to move in. Unlike above, these values can be set to 0 with no issue.
	rowShiftOffset = _rowShiftOffset;
	columnShiftOffset = _columnShiftOffset;

	// Finally, set the initial menu visibility.
	alpha = _alpha;
}

void Menu::InitializeOptionParams(float_t _anchorX, float_t _anchorY, float_t _spacingX, float_t _spacingY, olc::Pixel _color, olc::Pixel _hoverColor, olc::Pixel _selColor, olc::Pixel _inactiveColor) {
	if (MENU_ARE_OPTIONS_ALLOWED)
		return; // Don't allow initialization of menu option parameters twice.
	flags |= FLAG_MENU_OPTIONS_ALLOWED;

	// Copy over the values that will determine the top-left of the visible menu element area.
	optionAnchorX = _anchorX;
	optionAnchorY = _anchorY;

	// Assign the total spacing in whole pixels between each element along each axis. Note that these don't consider the width/height 
	// of an option itself so that will need to be considered relative to the contents of the menu itself.
	optionSpacingX = _spacingX;
	optionSpacingY = _spacingY;

	// Assign the text color parameters for when a menu option is visible, being hovered over by the user's cursor, and when it has 
	// been selected by said user, respectively.
	optionColor			= _color;
	optionHoverColor	= _hoverColor;
	optionSelColor		= _selColor;
	optionInactiveColor = _inactiveColor;
}

void Menu::InitializeDescriptionParams(float_t _x, float_t _y) {
	if (!MENU_CAN_SHOW_DESCRIPTION)
		return;

	flags |= FLAG_MENU_SHOW_DESCRIPTIONS;
	optionDescriptionX = _x;
	optionDescriptionY = _y;
}

void Menu::UpdateCursor() {
	if (MENU_IS_BLOCKING_INPUT || !MENU_IS_INITIALIZED || !MENU_ARE_OPTIONS_ALLOWED)
		return; // Don't bother updating the cursor if the menu isn't even considered initialized.

	// Resetting the flag for a fast cursor movement and also setting the move time to 0 so the next press of a cursor input 
	// will result in instantaneous movement processing.
	if (MINPUT_NO_CURSOR_INPUTS) {
		flags |=  FLAG_MENU_WAIT_CURSOR;
		flags &= ~FLAG_MENU_FAST_CURSOR;
		cursorMoveTimer = 0.0f;
		return;
	}

	// The menu is waiting for the cursor's timer to run out. Without this, the cursor would update its position on every game
	// frame, and that would be completely unusable. Instead, a frame-independent amount of time in seconds is used.
	if (MENU_IS_CURSOR_WAITING) {
		cursorMoveTimer -= EngineCore::deltaTime;
		if (cursorMoveTimer <= 0.0f) { // Signal that the menu can move the cursor on the next frame.
			flags &= ~FLAG_MENU_WAIT_CURSOR;

			// Makes the cursor autoscroll update slightly slower on the first movement of the cursor. Every subsequent movement 
			// will be the same interval of time until the user releases all menu cursor movement inputs.
			if (MENU_IS_CURSOR_FAST) { 
				cursorMoveTimer = MENU_CURSOR_SHIFT_TIME;
			} else {
				flags |= FLAG_MENU_FAST_CURSOR;
				cursorMoveTimer = MENU_CURSOR_FIRST_SHIFT_TIME; 
			}
		}
		return;
	}
	flags |= FLAG_MENU_WAIT_CURSOR;

	// Determine the horizontal movement of the cursor by converting two booleans into 8-bit integers. Then, take the result of the
	// right cursor state and subtract the left cursor state from it. This will mean the value is +1 for cursor movement to the right,
	// -1 for movement to the left, and 0 for movement in either both or neither direction.
	int8_t _hMovement = int8_t(MINPUT_IS_RIGHT_ACTIVE) - int8_t(MINPUT_IS_LEFT_ACTIVE);
	if (menuWidth == 1ui8 || _hMovement == 0ui8)
		goto vertical_cursor_movement_logic; // No movement required; completely skip the horizontal cursor logic and move onto processing vertical cursor movement.

	// First, check if the horizontal movement occurred to the left, and then check if it occurred to the right.
	if (_hMovement == 0xFFi8) {
		uint8_t _curColumn = curOption % menuWidth;

		// Decrement the visible region's offset if is above 0 and the current column requires this shift to occur due to the offset
		// values set upon the menu's initialization.
		if (_curColumn == curVisibleColumnOffset + columnShiftOffset && curVisibleColumnOffset != 0ui8)
			curVisibleColumnOffset--;

		// The cursor needs to be wrapped around to the rightmost column of options.
		if (_curColumn == 0ui8) {
			curOption += (menuWidth - 1ui8);
			// The last row can be missing up to n minus one options compared to other rows, and this if statement will ensure the
			// cursor never exceeds that last row's actual length if the above addition to curOption happens to do that.
			if (curOption >= menuOptions.size())
				curOption = uint8_t(menuOptions.size() - 1ui64);

			// Update which current row the highlight menu option is on since this information will be important in determining how
			// to offset the visible region relative to the region shifting offset and whatnot.
			_curColumn = curOption % menuWidth;
			curVisibleColumnOffset = menuWidth - numVisibleColumns;

			// The current column is somewhere between the rightmost and leftmost region.
			if (curOption / menuWidth == menuHeight && _curColumn <= curVisibleColumnOffset + columnShiftOffset) { 
				curVisibleColumnOffset = _curColumn + columnShiftOffset - (numVisibleColumns - 1ui8);
				if (_curColumn < numVisibleColumns >> 1) // The column is on the left side of the menu; always set to 0.
					curVisibleColumnOffset = 0ui8;
			}

			// Since horizontal movement has been processed it's not required to increment or decrement curOption by _hMovement, so
			// that code as well as having to set _hMovement to 0 to achieve no addition or subtraction when we don't want it are 
			// avoided through this use of a goto.
			goto vertical_cursor_movement_logic;
		}
	} else if (_hMovement == 1i8) {
		uint8_t _curColumn = curOption % menuWidth;

		// If the highlighted option is one option away from being at the shifting offset; premptively increment the visible region's
		// offset to account for the cursor's movement, but before any wrapping logic is checked/executed.
		if (_curColumn == curVisibleColumnOffset + numVisibleColumns - columnShiftOffset - 1ui8 && curVisibleColumnOffset != menuWidth - numVisibleColumns)
			curVisibleColumnOffset++;

		// Wrapping the cursor to the leftmost option on the current row. The mod to the current option will automatically account for 
		// the bottom row not having a complete row of options. The visible region's offset is set to 0 to shift it back to the left.
		if (_curColumn == menuWidth - 1ui8 || curOption >= menuOptions.size() - 1ui8) {
			curOption -= curOption % menuWidth;
			curVisibleColumnOffset = 0ui8;

			// Allows skipping over attempting to add 0 to curOption's value and having to set _hMovement to 0 to achieve that 0 add.
			goto vertical_cursor_movement_logic;
		}
	}
	// Simply incrementing or decrementing the value by one relative to _hMovement (It can be either +1, 0, or -1).
	curOption += _hMovement;

vertical_cursor_movement_logic:
	// Similar to above, the keyboard's required inputs are checked for their state, and then those booleans are converted into two ints
	// that are either the value's +1 or -1. Then, the upward input state's value is subtracted from the downward state's; resulting in
	// the value being +1 for downward movement, -1 for upward, and 0 for both simultaneously or none.
	int8_t _vMovement = int8_t(MINPUT_IS_DOWN_ACTIVE) - int8_t(MINPUT_IS_UP_ACTIVE);
	if (menuHeight == 1ui8 || _vMovement == 0i8)
		return; // No other code after this, so the function can exit early instead of needing a goto.

	// First, check if the vertical movement occurred upward, and then check if it occurred downward.
	if (_vMovement == 0xFFi8) {
		uint8_t _curRow = curOption / menuWidth;

		// Decrement the visible region's vertical offset until it reaches 0 as long as the cursor is on the row that is considered the
		// shifting row from the calculation done involving the "rowShiftOffset" against the topmost row's index.
		if (_curRow == curVisibleRowOffset + rowShiftOffset && curVisibleRowOffset != 0ui8)
			curVisibleRowOffset--;

		// Wrapping back to the bottomost row within the menu. If the final row isn't fully occupied, the highlighted option will be on 
		// the second-to-last row instead to prevent an out-of-bounds error.
		if (_curRow == 0ui8) {
			curOption = (curOption % menuWidth) + (menuWidth * (menuHeight - 1ui8));
			if (curOption >= menuOptions.size())
				curOption -= menuWidth;

			// Since a row will only ever have one element absent from it in the case of a menu element count and width/height total
			// mismatch, the visible region's offset will always be set to the menu's height minus its visible region height. No other
			// checks or considerations are required aside from a check to see if the subtraction is "negative" which will default the
			// value to zero to avoid any issues.
			if (int8_t(menuHeight - numVisibleRows) > 0i8)	{ curVisibleRowOffset = menuHeight - numVisibleRows; } 
			else											{ curVisibleRowOffset = 0ui8; }
			return; // Similar to the goto used above, this will skip needing to set _vMovement to 0 and a 0 add to curOption.
		}
	} else if (_vMovement == 1i8) {
		uint8_t _curRow = curOption / menuWidth;

		// The current visible region's offset will be incremented on every downward movement so long as the cursor is currently on the
		// row that is calculated to be the shifting region offset (One is removed as well since "numVisibleRows" starts at 1 but internally
		// the rows start from 0) and the region isn't already showing the lowest menu elements.
		if (_curRow == curVisibleRowOffset + numVisibleRows - rowShiftOffset - 1ui8 && curVisibleRowOffset != menuHeight - numVisibleRows)
			curVisibleRowOffset++;

		// Since a column will only ever have menu's height value or the menu's height minus one, the code here will check for either
		// of those conditions and then wrap the highlighted option as well as the visible region back around to the topmost row should
		// eother of the condition be met.
		if (_curRow == menuHeight - 1ui8 || (_curRow == menuHeight - 2ui8 && uint16_t(curOption + menuWidth) >= menuOptions.size())) {
			curOption = curOption % menuWidth;
			curVisibleRowOffset = 0ui8;
			return; // Similar to the goto used above, this will skip needing to set _vMovement to 0 and a 0 add to curOption.
		}
	}
	curOption += menuWidth * _vMovement;
}

void Menu::RenderVisibleOptions(EngineCore* _engine) {
	if (!MENU_IS_INITIALIZED || !MENU_ARE_OPTIONS_ALLOWED)
		return;

	size_t _yyOffset	= curVisibleRowOffset;
	size_t _xxOffset	= curVisibleColumnOffset;
	size_t _index		= 0ui64;
	size_t _menuSize	= menuOptions.size();
	olc::Pixel _color	= optionColor;

	// Render the highlighted option's description if the menu has been set to render option descriptions.
	if (MENU_CAN_SHOW_DESCRIPTION)
		_engine->DrawStringDecal({ optionDescriptionX, optionDescriptionY }, menuOptions[curOption].description);

	// Loop through the region of currently visible menu options; rendering them all using their "DrawSelf" function. Out of
	// bounds elements are automatically ignored on the last row if the number of menu options doesn't match the amount needed
	// to completely populate said final row of options.
	for (uint8_t yy = 0ui8; yy < numVisibleRows; yy++) {	// Starts from 0 to ensure option positions arent't offset once the top-left visible option is no longer the 0th row/column.
		for (uint8_t xx = 0ui8; xx < numVisibleColumns; xx++) {
			_index = size_t(menuWidth * _yyOffset) + _xxOffset;
			if (_index >= _menuSize) 
				break;
			
			// Determine the color of the option relative to what index is currently being hovered over by the cursor, selected 
			// by the user, or simply visible (Active vs. Inactive will alter the default color). The option being inactive will
			// override all other color changes.
			if (!MOPTION_IS_ACTIVE(_index)) {
				_color = optionInactiveColor;
			} else { // Choose a color as normal for an active option.
				if (_index == selOption)		{ _color = optionSelColor; }
				else if (_index == curOption)	{ _color = optionHoverColor; }
				else							{ _color = optionColor; }
			}

			// Only bother attempting to render the menu option if it is possibly visible to the player (alpha 0 == completely transparent).
			if (menuOptions[_index].alpha > 0ui8) {
				_color.a = uint16_t(alpha + menuOptions[_index].alpha) / 2ui8; // Blend the alpha level of the current option and menu itself.
				_engine->DrawStringDecal(
					{	optionAnchorX + (optionSpacingX * xx) + menuOptions[_index].xPos,
						optionAnchorY + (optionSpacingY * yy) + menuOptions[_index].yPos },
					menuOptions[_index].text, 
					_color
				);
			}
			_xxOffset++;
		}
		_xxOffset = curVisibleColumnOffset;
		_yyOffset++;
	}
}

void Menu::PrepareForActivation(uint8_t _state) {
	SET_NEXT_STATE(_state);

	curOption = 0ui8; // Wipe whatever was selected by the user previously within the menu.
	selOption = MENU_SELECTION_INVALID;

	flags &= ~FLAG_MENU_BLOCK_INPUT; // Ensure the menu is visible and can also accept input from the user.
	flags |= FLAG_MENU_VISIBLE | FLAG_MENU_ACTIVE_STATE;
}

void Menu::PrepareForDeactivation() {
	curState = STATE_INVALID;	// Bypass the wait for the end of the frame and just update it instantly upon deactivation.
	nextState = STATE_INVALID;

	inputFlags = 0u; // Clear out any active input flags upon deactivation of the menu.
	prevInputFlags = 0u;

	flags |= FLAG_MENU_BLOCK_INPUT; // Make sure the menu can no longer accept input from the user and is also switched to invisible.
	flags &= ~(FLAG_MENU_VISIBLE | FLAG_MENU_ACTIVE_STATE);
}

bool Menu::StateDefault() {
	// Capture what option was selected so it can be parsed in the next state, but only if the option is considered selectable.
	if (MINPUT_IS_SELECT_PRESSED && MOPTION_IS_SELECTABLE(curOption)) {
		SET_NEXT_STATE(STATE_MENU_PROCESS_SELECTION);
		selOption = curOption;
		return true;
	}

	UpdateCursor();
	return true;
}