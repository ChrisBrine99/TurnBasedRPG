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
//	
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

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for simplifying the typing required to check for an object's flag (within that object itself) bit to see if it is set to 0	//
//  or 1; then determining the course to take after that.																				//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define OBJ_IS_PERSISTENT				(flags & FLAG_OBJ_PERSISTENT)
#define OBJ_IS_INVINCIBLE				(flags & FLAG_OBJ_INVINCIBLE)
#define OBJ_IS_DESTROYED				(flags & FLAG_OBJ_DESTROYED) && !OBJ_IS_INVINCIBLE
#define OBJ_IS_ACTIVE					(flags & FLAG_OBJ_ACTIVE)
#define OBJ_IS_VISIBLE					(flags & FLAG_OBJ_VISIBLE) && OBJ_IS_ACTIVE

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Defines for the number that is used to reference an object during creation, so the object manager knows which one to initialize.	//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define ID_OBJECT_PLAYER				0x0000ui16

#endif