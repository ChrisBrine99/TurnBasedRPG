#include "ObjPlayer.hpp"

#include "../../singleton/DataManager.hpp"
#include "../../singleton/EngineCore.hpp"
#include "../../singleton/GameSettings.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Flags that are unique to the player object. They will utilize any bits not currently occupied by inherited flags.					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_PLYR_MOVING				0x40000000ui32
#define FLAG_PLYR_BLOCK_INPUT			0x80000000ui32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Check defines for the flags that are unique to the player object.																	//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define PLYR_IS_MOVING					(flags & FLAG_PLYR_MOVING)
#define PLYR_IS_BLOCKING_INPUT			(flags & FLAG_PLYR_BLOCK_INPUT)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Index values for the state functions that the player can utilize throughout the game.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define STATE_PLAYER_DEFAULT			0ui8

ObjPlayer::ObjPlayer(float_t _x, float_t _y, uint16_t _index, size_t _id) :
	DynamicObject(_x, _y, _index, _id, 0.2f, 1.6f),
	inputFlags(0ui32),
	prevInputFlags(0ui32)
{}

bool ObjPlayer::OnUserCreate() {
	spritesheet = ADD_SPRITE(SPR_PLAYER, "res/graphics/test_spritesheet.png");
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
	float_t _delta = EngineCore::deltaTime;

	if (OINPUT_ANY_DIRECTION_ACTIVE) { // Accelerating the player and setting their movement direction.
		float_t _xMovement	= float_t(OINPUT_IS_RIGHT_ACTIVE) - float_t(OINPUT_IS_LEFT_ACTIVE);
		float_t _yMovement	= float_t(OINPUT_IS_DOWN_ACTIVE) - float_t(OINPUT_IS_UP_ACTIVE);
		direction			= PointDirection(0.0f, 0.0f, _xMovement, _yMovement);

		speed += accel * _delta;
		if (speed > maxSpeed)
			speed = maxSpeed;
		flags |= FLAG_PLYR_MOVING;
	} else if (speed > 0.0f) { // Decelerating the player.
		speed -= accel * _delta;
		if (speed < 0.0f) {
			flags &= ~FLAG_PLYR_MOVING;
			speed = 0.0f;
		}
	}

	MoveAndCollide(LengthDirX(speed, direction), LengthDirY(speed, direction), _delta);
	return true;
}