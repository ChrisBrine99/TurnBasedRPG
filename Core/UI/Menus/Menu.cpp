#include "Menu.hpp"

#include "../../Singletons/GameSettings.hpp"

Menu::Menu() :
	xPos(0),
	yPos(0),
	upperMenu(nullptr),
	subMenu(nullptr),
	auxSelect(""),
	auxReturn(""),
	flags(0ui32),
	inputFlags(0ui32),
	prevInputFlags(0ui32),
	curState(INVALID_STATE),
	nextState(INVALID_STATE),
	lastState(INVALID_STATE),
	menuOptions(),
	curOption(0ui8),
	selOption(0xFFui8),
	auxSelOption(0xFFui8),
	cursorMoveTimer(0.0f),
	numVisibleRows(0ui8),
	numVisibleColumns(0ui8),
	curVisibleRowOffset(0ui8),
	curVisibleColumnOffset(0ui8),
	rowShiftOffset(0ui8),
	columnShiftOffset(0ui8),
	menuWidth(0ui8),
	menuHeight(0ui8),
	optionAnchorX(0),
	optionAnchorY(0),
	optionDescriptionX(0),
	optionDescriptionY(0),
	optionSpacingX(0),
	optionSpacingY(0),
	alpha(0ui8)
{ // Reserve a small portion of memory to store the pointers for the menu options that can exist within a given menu.
	menuOptions.reserve(5ui64);
}

bool Menu::OnUserDestroy() {
	if (subMenu) { // Call sub-menu's OnUserDestroy function before deleting it so it can clean itself and its sub-menus up as well.
		subMenu->OnUserDestroy();
		delete subMenu, subMenu = nullptr;
	}

	delete auxSelect, auxSelect = nullptr;
	delete auxReturn, auxReturn = nullptr;
	return true;
}

bool Menu::OnBeforeUserUpdate(float_t _deltaTime) {
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
		inputFlags |= (uint32_t(_settings->GetKeyInfo(auxSelect).bPressed) << 6u);
	}

	if (auxReturn != "") { // Only check for the auxiliary return button if one has been set by the menu.
		inputFlags |= (uint32_t(_settings->GetKeyInfo(auxReturn).bPressed) << 7u);
	}

	return true;
}

void Menu::OnAfterUserUpdate(float_t _deltaTime) 
{}

void Menu::AddOption(int32_t _xPos, int32_t _yPos, const std::string& _mainText, const std::string& _description, uint8_t _alpha, uint32_t _flags) {
	if (!FLAG_IS_MENU_ACTIVE || !FLAG_ARE_MENU_OPTIONS_ALLOWED) // Prevent menus from adding options before they've been fully initialized.
		return;

	MenuOption _newOption{
		_xPos,		_yPos,			// Top-left coordinates of the menu option on the screen.
		_mainText,	_description,	// Text that represents the option for the user, and the optional description text that explains selecting it does.
		_flags,		_alpha
	};

	menuOptions.push_back(_newOption);
	menuHeight = uint8_t(menuOptions.size()) / menuWidth; // Integer division automatically rounds this down through truncation which is what we want.
}

void Menu::RemoveOption(size_t _index) {
	if (_index >= menuOptions.size() || !FLAG_IS_MENU_ACTIVE)
		return;
	menuOptions.erase(menuOptions.begin() + _index);
	menuHeight = uint8_t(menuOptions.size()) / menuWidth;
}

void Menu::SetOptionFlags(size_t _index, uint32_t _flags, bool _overwrite) {
	if (_index >= menuOptions.size())
		return;

	if (_overwrite) { menuOptions[_index].flags  = _flags; }
	else			{ menuOptions[_index].flags |= _flags; }
}

void Menu::InitializeParams(uint8_t _state, uint8_t _width, uint8_t _visibleRows, uint8_t _visibleColumns, uint8_t _rowShiftOffset, uint8_t _columnShiftOffset, uint8_t _alpha, uint32_t _flags) {
	if (FLAG_IS_MENU_INITIALIZED)
		return; // Don't allow initialization of a menu twice.
	flags = _flags | FLAG_MENU_INITIALIZED;
	SET_NEXT_STATE(_state);

	// Assign the menu's height, but prevent it from being 0.
	menuWidth = std::max(_width, 0x01ui8);

	// Assign the menu's total number of visible options along the x and y axes. Much like the menu's width, these values cannot be set to 0.
	numVisibleRows = std::max(_visibleRows, 0x01ui8);
	numVisibleColumns = std::max(_visibleColumns, 0x01ui8);

	// Assign the buffer region between the cursor and edge of the visible portion of the menu before the options get shifted by one row/column 
	// relative to the direction the cursor happens to move in. Unlike above, these values can be set to 0 with no issue.
	rowShiftOffset = _rowShiftOffset;
	columnShiftOffset = _columnShiftOffset;

	// Finally, set the initial menu visibility.
	alpha = _alpha;
}

void Menu::InitializeOptionParams(int32_t _anchorX, int32_t _anchorY, int32_t _spacingX, int32_t _spacingY, olc::Pixel _color, olc::Pixel _hoverColor, olc::Pixel _selColor, bool _showDescription, int32_t _descriptionX, int32_t _descriptionY) {
	if (FLAG_ARE_MENU_OPTIONS_ALLOWED)
		return; // Don't allow initialization of menu option parameters twice.
	flags |= FLAG_MENU_OPTIONS_ALLOWED;

	// Copy over the values that will determine the top-left of the visible menu element area.
	optionAnchorX = _anchorX;
	optionAnchorY = _anchorY;

	// Assign the total spacing in whole pixels between each element along each axis. Note that these don't consider the width/height of an option 
	// itself so that will need to be considered relative to the contents of the menu itself.
	optionSpacingX = _spacingX;
	optionSpacingY = _spacingY;

	// Assign the text color parameters for when a menu option is visible, being hovered over by the user's cursor, and when it has been selected
	// by said user, respectively.
	optionColor = _color;
	optionHoverColor = _hoverColor;
	optionSelColor = _selColor;

	// Finally, set the position of an option's descriptive text on the screen IF the menu is set to display option descriptions.
	if (_showDescription) {
		flags |= FLAG_MENU_SHOW_DESCRIPTIONS;
		optionDescriptionX = _descriptionX;
		optionDescriptionY = _descriptionY;
	}
}

void Menu::UpdateCursor(float_t _deltaTime) {
	if (!FLAG_IS_MENU_INITIALIZED || !FLAG_ARE_MENU_OPTIONS_ALLOWED)
		return; // Don't bother updating the cursor if the menu isn't even considered initialized.

	// Resetting the flag for a fast cursor movement and also setting the move time to 0 so the next press of a cursor input will result in 
	// instantaneous movement processing.
	if (FLAG_NO_MENU_MOVE_INPUTS) {
		flags |=  FLAG_MENU_WAIT_CURSOR;
		flags &= ~FLAG_MENU_FAST_CURSOR;
		cursorMoveTimer = 0.0f;
		return;
	}

	if (FLAG_IS_MENU_CURSOR_WAITING) {
		cursorMoveTimer -= _deltaTime;
		if (cursorMoveTimer <= 0.0f) { // Signal that the menu can move the cursor on the next frame.
			flags &= ~FLAG_MENU_WAIT_CURSOR;

			// Makes the cursor autoscroll update slightly slower on the first movement of the cursor. Every subsequent movement will be the same
			// interval of time until the user releases all menu cursor movement inputs.
			if (FLAG_IS_MENU_CURSOR_FAST) { 
				cursorMoveTimer = MENU_CURSOR_SHIFT_TIME;
			} else {
				flags |= FLAG_MENU_FAST_CURSOR;
				cursorMoveTimer = MENU_CURSOR_FIRST_SHIFT_TIME; 
			}
		}
		return;
	}
	flags |= FLAG_MENU_WAIT_CURSOR;

	int8_t _hMovement = int8_t(FLAG_IS_MENU_RIGHT_ACTIVE) - int8_t(FLAG_IS_MENU_LEFT_ACTIVE);
	if (menuWidth != 0ui8 && _hMovement != 0i8) {
		uint8_t _curColumn = curOption % menuWidth;
		std::cout << "Column: " << uint32_t(_curColumn) << std::endl;
		if (_hMovement == 0xFFi8 && _curColumn == 0ui8) {
			curOption += (menuWidth - 1ui8);
			if (curOption >= menuOptions.size())
				curOption = uint8_t(menuOptions.size() - 1ui64);
			std::cout << "Wrapped around to the rightmost column." << std::endl;
		} else if (_hMovement == 1i8 && (_curColumn == menuWidth - 1ui8 || uint16_t(curOption + 1ui8) >= menuOptions.size())) {
			curOption -= curOption % menuWidth;
			std::cout << "Wrapped around to the leftmost column." << std::endl;
		} else {
			curOption += _hMovement;
		}
	}

	int8_t _vMovement = int8_t(FLAG_IS_MENU_DOWN_ACTIVE) - int8_t(FLAG_IS_MENU_UP_ACTIVE);
	if (menuHeight != 0ui8 && _vMovement != 0i8) {
		uint8_t _curRow	= curOption / menuWidth;
		std::cout << "Row: " << uint32_t(_curRow) << std::endl;
		if (_vMovement == 0xFFi8 && _curRow == 0ui8) {
			curOption = curOption % menuWidth + (menuWidth * menuHeight);
			if (curOption >= menuOptions.size())
				curOption -= menuWidth; // Shifts up by one column.
			std::cout << "Wrapped around to the bottomost row." << std::endl;
		} else if (_vMovement == 1i8 && (_curRow == menuHeight || uint16_t(curOption + menuWidth) >= menuOptions.size())) {
			curOption = curOption % menuWidth;
			std::cout << "Wrapped around to the topmost row." << std::endl;
		} else {
			if (_vMovement == 1i8)	{ curOption += menuWidth; }
			else					{ curOption -= menuWidth; }
		}
	}

	std::cout << "Currrent Option: " << uint32_t(curOption) << std::endl;
}

void Menu::RenderVisibleOptions(float_t _deltaTime) {
	if (!FLAG_IS_MENU_VISIBLE || menuWidth == 0ui8 || menuHeight < 0ui8)
		return;

	EngineCore* _engine	= GET_SINGLETON(EngineCore);
	size_t _yyOffset	= curVisibleRowOffset;
	size_t _xxOffset	= curVisibleColumnOffset;
	size_t _index		= 0ui64;
	size_t _menuSize	= menuOptions.size();

	// Loop through the region of currently visible menu options; rendering them all using their "DrawSelf" function. Out of
	// bounds elements are automatically ignored on the last row if the number of menu options doesn't match the amount needed
	// to completely populate said final row of options.
	for (uint8_t yy = 0ui8; yy < menuHeight; yy++) {	// Starts from 0 to ensure option positions arent't offset once the top-left visible option is no longer the 0th row/column.
		for (uint8_t xx = 0ui8; xx < menuWidth; xx++) {
			_index = size_t(menuWidth * _yyOffset) + _xxOffset;
			if (_index == _menuSize)
				break;
			
			menuOptions[_index].DrawSelf(_engine, optionAnchorX + (optionSpacingX * xx), optionAnchorY + (optionSpacingY * yy), alpha);
			_xxOffset++;
		}
		_yyOffset++;
	}
}