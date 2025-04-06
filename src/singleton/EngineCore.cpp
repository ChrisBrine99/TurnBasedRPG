#include "EngineCore.hpp"
INIT_SINGLETON_CPP(EngineCore)

#include "../extension/EngineCoreExt.hpp"
#include "DataManager.hpp"
#include "GameSettings.hpp"
#include "MenuManager.hpp"
#include "ObjectManager.hpp"
#include "PartyManager.hpp"
#include "SceneManager.hpp"
#include "../utility/UtilityFunctions.hpp"

uint32_t	EngineCore::s_HudLayer			= 0ui32;
uint32_t	EngineCore::s_ObjectLayer		= 0ui32;
uint32_t	EngineCore::s_BackgroundLayer	= 0ui32;
float_t		EngineCore::deltaTime			= 0.0f;

EngineCore::EngineCore() :
	engineExt(new EngineCoreExt(true)),
	sPerformanceData("fps          0\nmemory usage 0 bytes\ncur allocs   0"),
	svPerformanceData(sPerformanceData),
	updateTimer(0.0f)
{ // Sets the name for the application.
	sAppName = "Turn Based RPG";
}

bool EngineCore::OnUserCreate() {
	CALL_SINGLETON_CREATE(DataManager, OnUserCreate);
	CALL_SINGLETON_CREATE(SceneManager, OnUserCreate);

	s_ObjectLayer		= CreateLayer();
	s_BackgroundLayer	= CreateLayer();

	EnableLayer(s_ObjectLayer, true);
	EnableLayer(s_BackgroundLayer, true);
	return true;
}

bool EngineCore::OnUserDestroy() {
	delete engineExt, engineExt = nullptr;

	CALL_SINGLETON_DESTROY(SceneManager);
	CALL_SINGLETON_DESTROY(PartyManager);
	CALL_SINGLETON_DESTROY(ObjectManager);
	CALL_SINGLETON_DESTROY(MenuManager);
	CALL_SINGLETON_DESTROY(DataManager);
	CALL_SINGLETON_DESTROY(GameSettings);
	return true;
}

bool EngineCore::OnUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	CALL_SINGLETON_UPDATE(ObjectManager, OnUserUpdate);
	CALL_SINGLETON_UPDATE(SceneManager, OnUserUpdate);
	CALL_SINGLETON_UPDATE(MenuManager, OnUserUpdate);

	updateTimer += _deltaTime;
	if (updateTimer >= 1.0f) {
		updateTimer -= 1.0f;
		sPerformanceData	= "fps          " + std::to_string(GetFPS()) + "\nmemory usage " + std::to_string(s_BytesAllocated) + " bytes\ncur allocs   " + std::to_string(s_Allocations);
		svPerformanceData	= sPerformanceData;
	}
	return true;
}

bool EngineCore::OnUserRender(float_t _deltaTime) {
	(void)(_deltaTime);
	EngineCore* _engine = this; // Required for singletons to work with having to add an additional argument.

	Clear(COLOR_BLANK);
	SetPixelMode(olc::Pixel::ALPHA);
	CALL_SINGLETON_RENDER(MenuManager);

	SetDrawTarget(s_ObjectLayer);
	Clear(COLOR_BLANK);
	CALL_SINGLETON_RENDER(ObjectManager);

	SetDrawTarget(s_BackgroundLayer);
	Clear(COLOR_BLACK);
	SetPixelMode(olc::Pixel::NORMAL);
	CALL_SINGLETON_RENDER(SceneManager);

	SetDrawTarget(s_HudLayer);
	DrawString(5i32, 5i32, svPerformanceData.data());
	return true;
}