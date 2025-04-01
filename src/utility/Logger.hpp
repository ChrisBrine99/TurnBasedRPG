#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdint>
#include <stdio.h>
#include <string>

#define LOG_COLOR_BLACK					0x00ui8
#define LOG_COLOR_RED					0x01ui8
#define LOG_COLOR_GREEN					0x02ui8
#define LOG_COLOR_YELLOW				0x03ui8
#define LOG_COLOR_BLUE					0x04ui8
#define LOG_COLOR_MAGENTA				0x05ui8
#define LOG_COLOR_CYAN					0x06ui8
#define LOG_COLOR_WHITE					0x07ui8
#define LOG_COLOR_BRIGHT_BLACK			0x10ui8
#define LOG_COLOR_BRIGHT_RED			0x11ui8
#define LOG_COLOR_BRIGHT_GREEN			0x12ui8
#define LOG_COLOR_BRIGHT_YELLOW			0x13ui8
#define LOG_COLOR_BRIGHT_BLUE			0x14ui8
#define LOG_COLOR_BRIGHT_MAGENTA		0x15ui8
#define LOG_COLOR_BRIGHT_CYAN			0x16ui8
#define LOG_COLOR_BRIGHT_WHITE			0x17ui8

#define LOG_TRACE(_message, ...)		__log("TRACE -- ",		_message, LOG_COLOR_GREEN,		##__VA_ARGS__)
#define LOG_WARN(_message, ...)			__log("WARNING -- ",	_message, LOG_COLOR_YELLOW,		##__VA_ARGS__)
#define LOG_ERROR(_message, ...)		__log("ERROR -- ",		_message, LOG_COLOR_RED,		##__VA_ARGS__)

template<typename ...Args>
void __log(const char* _prefix, const char* _message, uint8_t _color, Args... _args) {
	std::string _textColor;
	switch (_color) {
	default:
	case LOG_COLOR_BLACK:				_textColor = "\033[30m";	break;
	case LOG_COLOR_RED:					_textColor = "\033[31m";	break;
	case LOG_COLOR_GREEN:				_textColor = "\033[32m";	break;
	case LOG_COLOR_YELLOW:				_textColor = "\033[33m";	break;
	case LOG_COLOR_BLUE:				_textColor = "\033[34m";	break;
	case LOG_COLOR_MAGENTA:				_textColor = "\033[35m";	break;
	case LOG_COLOR_CYAN:				_textColor = "\033[36m";	break;
	case LOG_COLOR_WHITE:				_textColor = "\033[37m";	break;
	case LOG_COLOR_BRIGHT_BLACK:		_textColor = "\033[90m";	break;
	case LOG_COLOR_BRIGHT_RED:			_textColor = "\033[91m";	break;
	case LOG_COLOR_BRIGHT_GREEN:		_textColor = "\033[92m";	break;
	case LOG_COLOR_BRIGHT_YELLOW:		_textColor = "\033[93m";	break;
	case LOG_COLOR_BRIGHT_BLUE:			_textColor = "\033[94m";	break;
	case LOG_COLOR_BRIGHT_MAGENTA:		_textColor = "\033[95m";	break;
	case LOG_COLOR_BRIGHT_CYAN:			_textColor = "\033[96m";	break;
	case LOG_COLOR_BRIGHT_WHITE:		_textColor = "\033[97m";	break;
	}

	std::cout << _textColor << _prefix << _message << "\033[0m" << std::endl;
}

#endif