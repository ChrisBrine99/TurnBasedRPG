#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

#include "../../ThirdParty/olcPixelGameEngine.hpp"
#include "../Utils/GeneralMacros.hpp"

class EngineCoreExt;
class EngineCore : public olc::PixelGameEngine {
	// Singleton Initialization/Constructor Declaration
	INIT_SINGLETON_HPP(EngineCore)
	EngineCore();
public: // Main Engine Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime);

private: // Hidden Member Variable Declarations
	EngineCoreExt* engineExt;
};

class EngineCoreExt : public olc::PGEX {
public: // Constructor and Destructor Declaration
	EngineCoreExt(bool _hook) : PGEX(_hook) {}
	~EngineCoreExt() {};

public: // Overridden Function Declarations
	void OnBeforeUserCreate() override;
	void OnAfterUserCreate() override;
	bool OnBeforeUserUpdate(float_t& _elapsedTime) override;
	void OnAfterUserUpdate(float_t _elapsedTime) override;
};

#endif