#ifndef TILE_HPP
#define TILE_HPP

#include <cstdint>

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Define that signifies whenever a tile hasn't had a valid index within the map's spritesheet set for it OR if the code ever tries	//
//	to access Tiles instances outside of the current map's boundaries.																	//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define TILE_INDEX_UNDEFINED			0xFFFFFFFFui32

struct Tile {
	uint32_t	index;
	uint32_t	flags;
};

#endif