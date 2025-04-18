#ifndef BOUNDING_BOX_HPP
#define BOUNCING_BOX_HPP

#include <cstdint>

struct BoundingBox {
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;

public: // Constructor/Destructor Declarations
	BoundingBox() = delete;
	BoundingBox(int32_t _x, int32_t _y, int32_t _width, int32_t _height) :
		x(_x),
		y(_y),
		width(_width),
		height(_height) 
	{}
	BoundingBox(const BoundingBox& _other) = delete;
	~BoundingBox() = default;

public: // Publicly Accessible Utility Function Declarations
	inline bool PointCollision(int32_t _x, int32_t _y) const {
		return (_x >= x && _y > y && _x <= x + width && _y <= y + height); 
	}

	inline bool BoxCollision(BoundingBox& _other) const {
		return (x < _other.x + _other.width && x + width > _other.x &&
				y < _other.y + _other.height && y + height > _other.y);
	}
};

#endif