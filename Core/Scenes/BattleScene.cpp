#include "BattleScene.hpp"

#include "../Singletons/BattleManager.hpp"
#include "../Singletons/DataManager.hpp"
#include "../Singletons/PartyManager.hpp"
#include "../UI/Menus/Battle/BattleMainMenu.hpp"

BattleScene::BattleScene(uint32_t _index) :
	Scene(_index),
	test(new BattleMainMenu())
{}

bool BattleScene::OnUserCreate() {
	if (!GET_SINGLETON(DataManager)->OnUserCreate())
		return false;

	test->OnUserCreate();

	GET_SINGLETON(PartyManager)->AddPartyMember(ID_TEST_PLAYER);

	return GET_SINGLETON(BattleManager)->OnUserCreate();
}

bool BattleScene::OnUserDestroy() {
	test->OnUserDestroy();
	delete test, test = nullptr;
	return true;
}

bool BattleScene::OnUserUpdate(float_t _deltaTime) {
	test->OnUserUpdate(_deltaTime);
	return GET_SINGLETON(BattleManager)->OnUserUpdate(_deltaTime);
}

bool BattleScene::OnUserRender(float_t _deltaTime) {
	test->OnUserRender(_deltaTime);
	return true;
}

bool BattleScene::OnBeforeUserUpdate(float_t _deltaTime) {
	test->OnBeforeUserUpdate(_deltaTime);

	return GET_SINGLETON(BattleManager)->OnBeforeUserUpdate(_deltaTime);
}

void BattleScene::OnAfterUserUpdate(float_t _deltaTime) {
	GET_SINGLETON(BattleManager)->OnAfterUserUpdate(_deltaTime);
}