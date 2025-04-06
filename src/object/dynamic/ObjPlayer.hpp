#ifndef OBJ_PLAYER_HPP
#define OBJ_PLAYER_HPP

#include "DynamicObject.hpp"


#define ANMINST_TEST					0x0000ui16

class ObjPlayer : public DynamicObject {
public: // Constructor/Destructor Declarations
	ObjPlayer(float_t _x, float_t _y, uint16_t _index, size_t _id);
	~ObjPlayer() = default;

public: // Main Engine Function Overrides
	bool OnUserCreate() override;
	bool OnUserUpdate() override;

	bool OnBeforeUserUpdate() override;

private: // Hidden State Function Declarations
	bool StateDefault();

private: // Hidden Member Variable Declarations
	uint32_t inputFlags;
	uint32_t prevInputFlags;
};

#endif