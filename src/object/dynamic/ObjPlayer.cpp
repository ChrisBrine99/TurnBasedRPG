#include "ObjPlayer.hpp"

#include "../../singleton/DataManager.hpp"
#include "../../singleton/EngineCore.hpp"
#include "../../singleton/GameSettings.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Flags that are unique to the player object. They will utilize any bits not currently occupied by inherited flags.					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_PLYR_BLOCK_INPUT			0x80000000ui32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Check defines for the flags that are unique to the player object.																	//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define PLYR_IS_BLOCKING_INPUT			(flags & FLAG_PLYR_BLOCK_INPUT)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	INdex values for the state functions that the player can utilize throughout the game.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define STATE_PLAYER_DEFAULT			0ui8

ObjPlayer::ObjPlayer(float_t _x, float_t _y, uint16_t _index, size_t _id) :
	DynamicObject(_x, _y, _index, _id, 0.3f, 0.3f, 1.5f, 1.5f),
	inputFlags(0ui32),
	prevInputFlags(0ui32)
{}

bool ObjPlayer::OnUserCreate() {
	spritesheet = GET_SINGLETON(DataManager)->LoadSprite(SPR_PLAYER, "res/graphics/test_spritesheet.png");
	if (!spritesheet) { return false; }

	AddAnimation(0ui8, 16.0f, 16.0f, 6.0f, {
		{   0.0f,  0.0f },
		{  16.0f,  0.0f },
		{  32.0f,  0.0f },
		{  48.0f,  0.0f },
		{  64.0f,  0.0f },
		{  96.0f,  0.0f },
		{ 112.0f,  0.0f }
	});
	nextAnimID = 0ui8;

	SET_NEXT_STATE(STATE_PLAYER_DEFAULT);
	flags |= FLAG_OBJ_ACTIVE | FLAG_OBJ_VISIBLE;
	return true;
}

bool ObjPlayer::OnUserUpdate() {
	if (OBJ_IS_DESTROYED)
		return true;

	switch (curState) {
	case STATE_INVALID:				return true;
	case STATE_PLAYER_DEFAULT:		return StateDefault();
	}

	return false;
}

bool ObjPlayer::OnBeforeUserUpdate() {
	if (PLYR_IS_BLOCKING_INPUT) {
		inputFlags = 0ui32;
		return true;
	}
	prevInputFlags = inputFlags;

	GameSettings* _settings = GET_SINGLETON(GameSettings);
	inputFlags = 
		(uint32_t(_settings->GetKeyInfo(INPUT_MOVE_RIGHT).bHeld)			  ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_MOVE_LEFT).bHeld)			<< 1u ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_MOVE_UP).bHeld)			<< 2u ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_MOVE_DOWN).bHeld)			<< 3u ) |
		(uint32_t(_settings->GetKeyInfo(INPUT_INTERACT).bPressed)		<< 4u );
	return true;
}

bool ObjPlayer::StateDefault() {
	

	UpdateMovementValues(0.0f);
	return true;
}