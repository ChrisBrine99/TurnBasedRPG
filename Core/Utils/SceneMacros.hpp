#ifndef SCENE_MACROS_HPP
#define SCENE_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that explain what each number represents to the StateManager. In this case, they represent the various scenes that can be	//
//	active or loaded at once; as well as default value indicating a state that is invalid to prevent undefined behavior.				//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define BATTLE_SCENE_INDEX				0x00000010ui32
#define INVALID_SCENE_INDEX				0xFFFFFFFFui32

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Makes sense of the numbers that represent the SceneManager's various states.														//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_SCENE_CHANGE				0x00000001ui32

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that allow the SceneManager to check specific states of flags stored within the "flags" variable. This cause certain code	//
//	to execute	and/or certain parts of the SceneManager's functionality to be enabled or disabled on-the-fly.							//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define SCENE_SHOULD_CHANGE				(flags & FLAG_SCENE_CHANGE)

#endif