#ifndef TEXT_ELEMENT_HPP
#define TEXT_ELEMENT_HPP

#include "../../../singleton/EngineCore.hpp"
#include "../../../utility/GeneralMacros.hpp"

struct TextElement {
	float_t		x;
	float_t		y;
	float_t		xOffset;
	float_t		yOffset;
	std::string text;
	olc::Pixel	color;
	float_t		scale;

public: // Constructor/Destructor Definitions/Declarations
	TextElement(float_t _x, float_t _y, const std::string& _text, olc::Pixel _color = COLOR_WHITE, float_t _scale = 1.0f) :
		x(_x),
		y(_y),
		xOffset(0.0f),
		yOffset(0.0f),
		text(_text),
		color(_color),
		scale(_scale)
	{}
	TextElement(const TextElement& _other) = delete;
	~TextElement() = default;

public: // Publicly Accessible Utility Function Definitions
	inline void OnUserRender(EngineCore* _engine) const {
		_engine->DrawStringDecal({ std::floor(x + xOffset), std::floor(y + yOffset) }, text, color, { scale, scale });
	}
};

#endif