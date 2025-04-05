#ifndef MENU_OPTION_HPP
#define MENU_OPTION_HPP

#include <string>

struct MenuOption {
	float_t		xPos;
	float_t		yPos;
	std::string	text;
	std::string	description;
	uint32_t	flags;
	uint8_t		alpha;
};

#endif