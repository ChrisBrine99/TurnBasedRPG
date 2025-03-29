#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

#include "../../third_party/olcPixelGameEngine.hpp"
#include "../utility/GeneralMacros.hpp"

class EngineCoreExt;
class EngineCore : public olc::PixelGameEngine {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(EngineCore)
public: // Main Engine Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

public: // Publicly Accessible Member Variable Declarations
	static uint32_t s_HudLayer;
	static uint32_t s_ObjectLayer;
	static uint32_t s_BackgroundLayer;

private: // Hidden Member Variable Declarations
	const EngineCoreExt* engineExt;
};

#endif