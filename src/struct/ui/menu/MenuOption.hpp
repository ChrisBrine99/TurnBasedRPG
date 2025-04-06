#ifndef MENU_OPTION_HPP
#define MENU_OPTION_HPP

#include <string>

struct MenuOption {
	int32_t		xPos;
	int32_t		yPos;
	std::string	text;
	std::string	description;
	uint32_t	flags;
	uint8_t		alpha;
};

#endif