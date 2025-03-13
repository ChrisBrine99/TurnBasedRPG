#include "EngineCore.hpp"
INIT_SINGLETON_CPP(EngineCore)

#include "BattleManager.hpp"
#include "DataManager.hpp"
#include "GameSettings.hpp"
#include "PartyManager.hpp"
#include "SceneManager.hpp"

EngineCore::EngineCore() {
	sAppName = "TurnBasedRPG";
	engineExt = new EngineCoreExt(true);
}

bool EngineCore::OnUserCreate() {
	if (!GET_SINGLETON(DataManager)->OnUserCreate())
		return false;

	return GET_SINGLETON(SceneManager)->OnUserCreate();
}

bool EngineCore::OnUserDestroy() {
	delete engineExt, engineExt = nullptr;

	GET_SINGLETON(BattleManager)->OnUserDestroy();
	GET_SINGLETON(DataManager)->OnUserDestroy();
	GET_SINGLETON(GameSettings)->OnUserDestroy();
	GET_SINGLETON(PartyManager)->OnUserDestroy();
	GET_SINGLETON(SceneManager)->OnUserDestroy();

	return true;
}

bool EngineCore::OnUserUpdate(float_t _deltaTime) {
	if (!GET_SINGLETON(SceneManager)->OnUserUpdate(_deltaTime))
		return false;

	return OnUserRender(_deltaTime);
}

bool EngineCore::OnUserRender(float_t _deltaTime) {
	Clear(olc::BLACK);
	return GET_SINGLETON(SceneManager)->OnUserRender(_deltaTime);
}

void EngineCoreExt::OnBeforeUserCreate() {
	if (!GET_SINGLETON(DataManager)->OnBeforeUserCreate()) {
		pge->olc_Terminate();
		return;
	}
	GET_SINGLETON(GameSettings)->OnBeforeUserCreate();
}

void EngineCoreExt::OnAfterUserCreate() {
	// Unused currently, but required.
}

bool EngineCoreExt::OnBeforeUserUpdate(float_t& _deltaTime) {
	if (GET_SINGLETON(SceneManager)->OnBeforeUserUpdate(_deltaTime)) 
		return false;

	pge->olc_Terminate();
	return true;
}

void EngineCoreExt::OnAfterUserUpdate(float_t _deltaTime) {
	if (!GET_SINGLETON(SceneManager)->OnAfterUserUpdate(_deltaTime))
		pge->olc_Terminate();
}

int main(int _argc, char *_argv[]) {
	if (GET_SINGLETON(EngineCore)->Construct(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, WINDOW_SCALE, WINDOW_SCALE)) {
		GET_SINGLETON(EngineCore)->Start();
		return EXIT_SUCCESS;
	}
	std::cout << "Program failed to launch" << std::endl;
	return EXIT_FAILURE;
}