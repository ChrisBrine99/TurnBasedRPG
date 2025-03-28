#ifndef OBJ_PLAYER_HPP
#define OBJ_PLAYER_HPP

#include "DynamicObject.hpp"

class ObjPlayer : public DynamicObject {
public: // Constructor/Destructor Declarations
	ObjPlayer(int32_t _x, int32_t _y, uint16_t _index, size_t _id);
	~ObjPlayer() = default;

public: // Main Engine Function Overrides
	bool OnUserCreate() override;
	bool OnUserUpdate(float_t _deltaTime) override;
};

#endif