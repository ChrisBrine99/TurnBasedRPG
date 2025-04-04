#ifndef MENU_MACROS_HPP
#define MENU_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Two defines for menu instances. The first is a simple define that handles the casting and creation of a new menu object, and the	//
//	second is for destroying that menu without having to type out "GET_SINGLETON" all the time.											//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define CREATE_NEW_MENU(_x)				(_x*)GET_SINGLETON(MenuManager)->CreateMenu(new _x())
#define DESTROY_MENU(_x, _type)			GET_SINGLETON(MenuManager)->DestroyMenu((_type*)_x)

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

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines for the default states that all menus have available to them. ANy indexes not found here can be used on a per menu basis.	//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define STATE_MENU_DEFAULT				0ui8
#define STATE_MENU_PROCESS_SELECTION	1ui8
// NOTE -- Values between 2 (0x02) and 254 (0xFE) are free for child menu states to use.

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for the intervals of time that the menu cursor must wait to move again as the user is holding a given direction (Right,		//
//	Left, Up, or Down, respectively) on the very first movement of the cursor as well as all subsequent movements until the user		//
//	releases the input for said direction.																								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define MENU_CURSOR_FIRST_SHIFT_TIME	30.0f
#define MENU_CURSOR_SHIFT_TIME			10.0f

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
// NOTE -- Bits 0x00000100ui32 and above are free to use for all children instances.

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that simplify the formatting required to check for each of a given menu's state-independent flags.							//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define MENU_IS_BLOCKING_INPUT			(flags & FLAG_MENU_BLOCK_INPUT)
#define MENU_IS_ACTIVE					(flags & FLAG_MENU_ACTIVE_STATE)
#define MENU_IS_VISIBLE					(flags & FLAG_MENU_VISIBLE)
#define MENU_IS_INITIALIZED				(flags & FLAG_MENU_INITIALIZED)
#define MENU_ARE_OPTIONS_ALLOWED		(flags & FLAG_MENU_OPTIONS_ALLOWED)
#define MENU_IS_CURSOR_WAITING			(flags & FLAG_MENU_WAIT_CURSOR)
#define MENU_CAN_SHOW_DESCRIPTION		(flags & FLAG_MENU_SHOW_DESCRIPTIONS)
#define MENU_IS_CURSOR_FAST				(flags & FLAG_MENU_FAST_CURSOR)

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

#define MINPUT_IS_RIGHT_ACTIVE			(inputFlags & FLAG_INPUT_MENU_RIGHT) && !(inputFlags & FLAG_INPUT_MENU_LEFT)
#define MINPUT_IS_LEFT_ACTIVE			(inputFlags & FLAG_INPUT_MENU_LEFT)	 && !(inputFlags & FLAG_INPUT_MENU_RIGHT)
#define MINPUT_IS_UP_ACTIVE				(inputFlags & FLAG_INPUT_MENU_UP)	 && !(inputFlags & FLAG_INPUT_MENU_DOWN)
#define MINPUT_IS_DOWN_ACTIVE			(inputFlags & FLAG_INPUT_MENU_DOWN)	 && !(inputFlags & FLAG_INPUT_MENU_UP)
#define MINPUT_IS_SELECT_ACTIVE			(inputFlags & FLAG_INPUT_MENU_SELECT)
#define MINPUT_IS_RETURN_ACTIVE			(inputFlags & FLAG_INPUT_MENU_RETURN)
#define MINPUT_IS_AUXSELECT_ACTIVE		(inputFlags & FLAG_INPUT_MENU_AUX_SELECT)
#define MINPUT_IS_AUXRETURN_ACTIVE		(inputFlags & FLAG_INPUT_MENU_AUX_RETURN)
#define MINPUT_NO_CURSOR_INPUTS			!(inputFlags & (FLAG_INPUT_MENU_RIGHT | FLAG_INPUT_MENU_LEFT | FLAG_INPUT_MENU_UP | FLAG_INPUT_MENU_DOWN))

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Special case checks that can see if the given input was pressed or released if those states are ever required.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define MINPUT_IS_RIGHT_PRESSED			MINPUT_IS_RIGHT_ACTIVE		&& !(prevInputFlags & FLAG_INPUT_MENU_RIGHT)
#define MINPUT_IS_LEFT_PRESSED			MINPUT_IS_LEFT_ACTIVE		&& !(prevInputFlags & FLAG_INPUT_MENU_LEFT)
#define MINPUT_IS_UP_PRESSED			MINPUT_IS_UP_ACTIVE			&& !(prevInputFlags & FLAG_INPUT_MENU_UP)
#define MINPUT_IS_DOWN_PRESSED			MINPUT_IS_DOWN_ACTIVE		&& !(prevInputFlags & FLAG_INPUT_MENU_DOWN)
#define MINPUT_IS_SELECT_PRESSED		MINPUT_IS_SELECT_ACTIVE		&& !(prevInputFlags & FLAG_INPUT_MENU_SELECT)
#define MINPUT_IS_RETURN_PRESSED		MINPUT_IS_RETURN_ACTIVE		&& !(prevInputFlags & FLAG_INPUT_MENU_RETURN)

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

#define MOPTION_IS_ACTIVE(_x)			(menuOptions[_x].flags & FLAG_MOPTION_ACTIVE_STATE)
#define MOPTION_IS_SELECTABLE(_x)		((menuOptions[_x].flags & FLAG_MOPTION_SELECTABLE) && (menuOptions[_x].flags & FLAG_MOPTION_ACTIVE_STATE))
#define MOPTION_IS_VISIBLE(_x)			(menuOptions[_x]flags & FLAG_MOPTION_VISIBLE)

#endif