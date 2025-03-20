#ifndef ENGINE_CORE_EXT_HPP
#define ENGINE_CORE_EXT_HPP

#include "../../ThirdParty/olcPixelGameEngine.hpp"

class EngineCoreExt : public olc::PGEX {
	friend class EngineCore;
public: // Constructor and Destructor Declaration
	EngineCoreExt(bool _hook);
	~EngineCoreExt() = default;

public: // Declarations To Prevent Copying of This Extension Class
	EngineCoreExt(const EngineCoreExt&) = delete;
	void operator=(const EngineCoreExt&) = delete;

public: // Overridden Function Declarations
	bool OnBeforeUserCreate() override;
	bool OnAfterUserCreate() override;
	bool OnBeforeUserUpdate(float_t _deltaTime) override;
	bool OnAfterUserUpdate(float_t _deltaTime) override;
};

#endif