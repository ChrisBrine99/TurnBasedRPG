#ifndef OBJECT_MACROS_HPP
#define OBJECT_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Since I don't want the object manager constantly allocating and de-allocating memory for the creation and deletion of object		//
//	instances during runtime, it will instead allocate and de-allocate in chunks of 16 whenever required.								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define OBJMNGR_RESERVE_SIZE			16ui64

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A define for the error value of an object ID. If any object happens to have this default value something went wrong during their	//
//	creation/initialization.																											//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define OBJMNGR_INVALID_INSTANCE_ID		0xFFFFFFFFFFFFFFFFui64

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines that simplify the code required to add and remove objects from the current scene.											//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define CREATE_OBJECT(_index, _x, _y)	GET_SINGLETON(ObjectManager)->AddObject(_index, _x, _y)
#define DESTROY_OBJECT(_id)				GET_SINGLETON(ObjectManager)->RemoveObject(_id)

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for the flags required by all standard objects. The bits not referenced here can be utilized for whatever purpose within	//
//	each child object.																													//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_OBJ_PERSISTENT				0x00000001ui32
#define FLAG_OBJ_INVINCIBLE				0x00000002ui32
#define FLAG_OBJ_DESTROYED				0x00000004ui32
#define FLAG_OBJ_ACTIVE					0x00000008ui32
#define FLAG_OBJ_VISIBLE				0x00000010ui32
#define FLAG_OBJ_ANIMATION_END			0x00000020ui32
#define FLAG_OBJ_SOLID					0x00000040ui32
#define FLAG_OBJ_COLLIDE_WITH_WORLD		0x00000080ui32

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for simplifying the typing required to check for an object's flag (within that object itself) bit to see if it is set to 0	//
//  or 1; then determining the course to take after that.																				//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define OBJ_IS_PERSISTENT				(flags & FLAG_OBJ_PERSISTENT)
#define OBJ_IS_INVINCIBLE				(flags & FLAG_OBJ_INVINCIBLE)
#define OBJ_IS_DESTROYED				(flags & FLAG_OBJ_DESTROYED) && !OBJ_IS_INVINCIBLE
#define OBJ_IS_ACTIVE					(flags & FLAG_OBJ_ACTIVE)
#define OBJ_IS_VISIBLE					(flags & FLAG_OBJ_VISIBLE) && OBJ_IS_ACTIVE
#define OBJ_DID_ANIMATION_END			(flags & FLAG_OBJ_ANIMATION_END)
#define OBJ_IS_SOLID					(flags & FLAG_OBJ_SOLID)
#define OBJ_CAN_COLLIDE_WITH_WORLD		(flags & FLAG_OBJ_COLLIDE_WITH_WORLD)

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for flags required by all dynamic objects. The bits not seen here or within the standard object flags are free to be used	//
//	on a per-dynamic object basis as required.																							//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_DOBJ_HOSTILE				0x00000080ui32

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for simplifying the typing required to check for a dynamic object's flag (within that object itself) bit to see if it is	//
//	set to 0 or 1; then determining the course to take after that.																		//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define DOBJ_IS_HOSTILE					(flags & FLAG_DOBJ_HOSTILE)

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for the input flags that can be utilized by player-controlled objects.														//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define FLAG_INPUT_OBJ_RIGHT			0x00000001ui32
#define FLAG_INPUT_OBJ_LEFT				0x00000002ui32
#define FLAG_INPUT_OBJ_UP				0x00000004ui32
#define FLAG_INPUT_OBJ_DOWN				0x00000008ui32
#define FLAG_INPUT_OBJ_INTERACT			0x00000010ui32

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Checks to see if the input flags utilized by player-controlled obnects have been pressed, released, or held.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define OINPUT_IS_RIGHT_ACTIVE			(inputFlags & FLAG_INPUT_OBJ_RIGHT)		&& !(inputFlags & FLAG_INPUT_OBJ_LEFT)
#define OINPUT_IS_LEFT_ACTIVE			(inputFlags & FLAG_INPUT_OBJ_LEFT)		&& !(inputFlags & FLAG_INPUT_OBJ_RIGHT)
#define OINPUT_IS_UP_ACTIVE				(inputFlags & FLAG_INPUT_OBJ_UP)		&& !(inputFlags & FLAG_INPUT_OBJ_DOWN)
#define OINPUT_IS_DOWN_ACTIVE			(inputFlags & FLAG_INPUT_OBJ_DOWN)		&& !(inputFlags & FLAG_INPUT_OBJ_UP)
#define OINPUT_IS_INTERACT_PRESSED		(inputFlags & FLAG_INPUT_OBJ_INTERACT)	&& !(prevInputFlags & FLAG_INPUT_OBJ_INTERACT)

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Special checks against the object's input flags for things that require more than two inputs during a single check.					//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define OINPUT_ANY_DIRECTION_ACTIVE		(OINPUT_IS_RIGHT_ACTIVE || OINPUT_IS_LEFT_ACTIVE || OINPUT_IS_UP_ACTIVE || OINPUT_IS_DOWN_ACTIVE)

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for the number that is used to reference an object during creation, so the object manager knows which one to initialize.	//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define OBJ_PLAYER						0x0000ui16

#endif