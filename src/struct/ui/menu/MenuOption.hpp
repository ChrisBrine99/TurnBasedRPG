#ifndef MENU_OPTION_HPP
#define MENU_OPTION_HPP

#include <cstdint>

struct MenuOption {
	int32_t			xPos;
	int32_t			yPos;
	const char*		text;
	const char*		description;
	uint32_t		flags;
	uint8_t			alpha;
};

#endif