#ifndef MAP_HPP
#define MAP_HPP

#include "../../singleton/EngineCore.hpp"
#include "Tile.hpp"

struct Map {
	olc::Decal*			tilesheet;
	std::vector<Tile>	tiles;

	const uint32_t		width;
	const uint32_t		height;

public: // Constructor/Destructor Definitions
	Map() = delete;
	Map(uint32_t _width, uint32_t _height, olc::Decal* _tilesheet = nullptr) : 
		tilesheet(_tilesheet),
		tiles(),
		width(_width),
		height(_height)
	{ // Allocate and instantiate the tiles required for the map given its dimensions.
		size_t _size = size_t(width * height);
		tiles.reserve(_size);
		for (uint64_t i = 0ui64; i < _size; i++)
			tiles.push_back({ TILE_INDEX_UNDEFINED, 0ui32 });
	}
	Map(const Map& _other) = delete;
	~Map() = default;

public: // Publicly Accessible Utility Function Definitions
	inline uint32_t GetTileIndex(uint32_t _x, uint32_t _y) const {
		if (_x < width && _y < height)
			return tiles[size_t(_y * width * _x)].index;
		return TILE_INDEX_UNDEFINED;
	}

	inline uint32_t GetTileFlags(uint32_t _x, uint32_t _y) const {
		if (_x < width && _y < height)
			return tiles[size_t(_y * width * _x)].flags;
		return 0ui32;
	}
};

#endif