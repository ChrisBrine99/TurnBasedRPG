#ifndef TEXT_ELEMENT_HPP
#define TEXT_ELEMENT_HPP

#include "../../../singleton/EngineCore.hpp"
#include "../../../utility/GeneralMacros.hpp"

struct TextElement {
	float_t		x;
	float_t		y;
	std::string text;
	olc::Pixel	color;
	float_t		scale;
	float_t		moveSpeed;
	float_t		lifetime;

public: // Constructor/Destructor Definitions/Declarations
	TextElement() :
		x(0.0f),
		y(0.0f),
		text(""),
		color(COLOR_WHITE),
		scale(1.0f),
		moveSpeed(0.0f),
		lifetime(0.0f)
	{}
	TextElement(float_t _x, float_t _y, const std::string& _text, float_t _moveSpeed, float_t _lifetime, olc::Pixel _color, float_t _scale)
		{ EnableElement(_x, _y, _text, _moveSpeed, _lifetime, _color, _scale); }
	TextElement(const TextElement& _other) = delete;
	~TextElement() = default;

public: // Publicly Accessible Utility Function Definitions
	inline void EnableElement(float_t _x, float_t _y, const std::string& _text, float_t _moveSpeed, float_t _lifetime = 0.0f, olc::Pixel _color = COLOR_WHITE, float_t _scale = 1.0f) {
		x			= _x;
		y			= _y;
		text		= _text;
		color		= _color;
		scale		= _scale;
		moveSpeed	= _moveSpeed;
		lifetime	= _lifetime;
	}

	inline bool OnUserRender(EngineCore* _engine, float_t _deltaTime) {
		if (lifetime == 0.0f)
			return true;

		lifetime -= _deltaTime;
		if (lifetime < 0.0f) {
			lifetime = 0.0f;
			return false;
		}

		if (moveSpeed != 0.0f) { y += moveSpeed * _deltaTime; }
		_engine->DrawString(int32_t(x), int32_t(y), text, color);
		return true;
	}
};

#endif