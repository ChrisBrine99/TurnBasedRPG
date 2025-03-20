#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

#include "../../ThirdParty/olcPixelGameEngine.hpp"
#include "../Utils/GeneralMacros.hpp"

class EngineCoreExt;
class EngineCore : public olc::PixelGameEngine {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(EngineCore)
public: // Main Engine Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

private: // Hidden Member Variable Declarations
	const EngineCoreExt* engineExt;
};

#endif