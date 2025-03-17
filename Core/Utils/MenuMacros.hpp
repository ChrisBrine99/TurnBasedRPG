#ifndef MENU_MACROS_HPP
#define MENU_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A simple macro that denotes the error value for indexes within the MenuManager's vector of existing menu instances.					//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define MENU_INVALID_INDEX				0xFFFFFFFFFFFFFFFFui64

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Determines how many options can be added to a menu before it will no longer accept any more.										//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define MAX_MENU_OPTIONS				0xFEui8

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	The default value for "selOption" which is also what is used to determine if nothing has been selected on the menu by the user.		//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define MENU_SELECTION_INVALID			0xFFui8

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for the intervals of time that the menu cursor must wait to move again as the user is holding a given direction (Right,		//
//	Left, Up, or Down, respectively) on the very first movement of the cursor as well as all subsequent movements until the user		//
//	releases the input for said direction.																								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define MENU_CURSOR_FIRST_SHIFT_TIME	0.65f
#define MENU_CURSOR_SHIFT_TIME			0.15f

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that detail what each bit within the "flags" variable for each menu represents in the context of the menu itself. The		//
//	values detailed here are flags that are required for each menu, and any remaining bits are free to use on a per-menu basis.			//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_MENU_BLOCK_INPUT			0x00000001ui32
#define FLAG_MENU_ACTIVE_STATE			0x00000002ui32
#define FLAG_MENU_VISIBLE				0x00000004ui32
#define FLAG_MENU_INITIALIZED			0x00000008ui32
#define FLAG_MENU_OPTIONS_ALLOWED		0x00000010ui32
#define FLAG_MENU_WAIT_CURSOR			0x00000020ui32
#define FLAG_MENU_SHOW_DESCRIPTIONS		0x00000040ui32
#define FLAG_MENU_FAST_CURSOR			0x00000080ui32

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that simplify the formatting required to check for each of a given menu's state-independent flags.							//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_MENU_BLOCKING_INPUT		(flags & FLAG_MENU_BLOCK_INPUT)
#define FLAG_IS_MENU_ACTIVE				(flags & FLAG_MENU_ACTIVE_STATE)
#define FLAG_IS_MENU_VISIBLE			(flags & FLAG_MENU_VISIBLE)
#define FLAG_IS_MENU_INITIALIZED		(flags & FLAG_MENU_INITIALIZED)
#define FLAG_ARE_MENU_OPTIONS_ALLOWED	(flags & FLAG_MENU_OPTIONS_ALLOWED)
#define FLAG_IS_MENU_CURSOR_WAITING		(flags & FLAG_MENU_WAIT_CURSOR)
#define FLAG_MENU_CAN_SHOW_DESCRIPTION	(flags & FLAG_MENU_SHOW_DESCRIPTIONS)
#define FLAG_IS_MENU_CURSOR_FAST		(flags & FLAG_MENU_FAST_CURSOR)

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that detail what each bit within the "inputFlags" variable for each menu represents in the context of the menu itself. The	//
//  values found here are simply the defaults that all menus will store by default.														//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_INPUT_MENU_RIGHT			0x00000001ui32
#define FLAG_INPUT_MENU_LEFT			0x00000002ui32
#define FLAG_INPUT_MENU_UP				0x00000004ui32
#define FLAG_INPUT_MENU_DOWN			0x00000008ui32
#define FLAG_INPUT_MENU_SELECT			0x00000010ui32
#define FLAG_INPUT_MENU_RETURN			0x00000020ui32
#define FLAG_INPUT_MENU_AUX_SELECT		0x00000040ui32
#define FLAG_INPUT_MENU_AUX_RETURN		0x00000080ui32
// NOTE -- Bits 0x00000100 onward are free to be utilized on a per-menu basis if required.

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that simplify the check required to see if a given input in the current menu is currently active or not. Note that input	//
//	for navigating the menu will inherently prevent movement in a direction if the exact opposite direciton's key is also held.			//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_IS_MENU_RIGHT_ACTIVE		(inputFlags & FLAG_INPUT_MENU_RIGHT) && !(inputFlags & FLAG_INPUT_MENU_LEFT)
#define FLAG_IS_MENU_LEFT_ACTIVE		(inputFlags & FLAG_INPUT_MENU_LEFT)	 && !(inputFlags & FLAG_INPUT_MENU_RIGHT)
#define FLAG_IS_MENU_UP_ACTIVE			(inputFlags & FLAG_INPUT_MENU_UP)	 && !(inputFlags & FLAG_INPUT_MENU_DOWN)
#define FLAG_IS_MENU_DOWN_ACTIVE		(inputFlags & FLAG_INPUT_MENU_DOWN)	 && !(inputFlags & FLAG_INPUT_MENU_UP)
#define FLAG_IS_MENU_SELECT_ACTIVE		(inputFlags & FLAG_INPUT_MENU_SELECT)
#define FLAG_IS_MENU_RETURN_ACTIVE		(inputFlags & FLAG_INPUT_MENU_RETURN)
#define FLAG_IS_MENU_AUX_SELECT_ACTIVE	(inputFlags & FLAG_INPUT_MENU_AUX_SELECT)
#define FLAG_IS_MENU_AUX_RETURN_ACTIVE	(inputFlags & FLAG_INPUT_MENU_AUX_RETURN)
#define FLAG_NO_MENU_MOVE_INPUTS		!(inputFlags & (FLAG_INPUT_MENU_RIGHT | FLAG_INPUT_MENU_LEFT | FLAG_INPUT_MENU_UP | FLAG_INPUT_MENU_DOWN))

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Special case checks that can see if the given cursor input was pressed or released if those states are ever required.				//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_IS_MENU_RIGHT_PRESSED		FLAG_IS_MENU_RIGHT_ACTIVE	&& !(prevInputFlags & FLAG_INPUT_MENU_RIGHT)
#define FLAG_IS_MENU_LEFT_PRESSED		FLAG_IS_MENU_LEFT_ACTIVE	&& !(prevInputFlags & FLAG_INPUT_MENU_LEFT)
#define FLAG_IS_MENU_UP_PRESSED			FLAG_IS_MENU_UP_ACTIVE		&& !(prevInputFlags & FLAG_INPUT_MENU_UP)
#define FLAG_IS_MENU_DOWN_PRESSED		FLAG_IS_MENU_DOWN_ACTIVE	&& !(prevInputFlags & FLAG_INPUT_MENU_DOWN)

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that represent the bit found within a menu option's "flags" variable. These can be toggled to change how the option works.	//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_MOPTION_ACTIVE_STATE		0x00000001ui32
#define FLAG_MOPTION_SELECTABLE			0x00000002ui32
#define FLAG_MOPTION_VISIBLE			0x00000004ui32

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that "condense" the code required to check the various flags that affect the functionality of individual menu options into	//
//	easy to use constants for use throughout the code.																				//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_IS_MOPTION_ACTIVE(_x)		(menuOptions[_x].flags & FLAG_MOPTION_ACTIVE_STATE)
#define FLAG_IS_MOPTION_SELECTABLE(_x)	((menuOptions[_x].flags & FLAG_MOPTION_SELECTABLE) && (menuOptions[_x].flags & FLAG_MOPTION_ACTIVE_STATE))
#define FLAG_IS_MOPTION_VISIBLE(_x)		(menuOptions[_x]flags & FLAG_MOPTION_VISIBLE)

#endif