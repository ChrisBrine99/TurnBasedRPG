#include "BattleScene.hpp"

#include "../Singletons/BattleManager.hpp"
#include "../Singletons/PartyManager.hpp"

#include "../Singletons/EngineCore.hpp"

BattleScene::BattleScene(uint32_t _index) :
	Scene(_index)
{}

bool BattleScene::OnUserCreate() {
	GET_SINGLETON(PartyManager)->AddPartyMember(ID_TEST_PLAYER);
	GET_SINGLETON(PartyManager)->AddToPartyRoster(ID_TEST_PLAYER);
	GET_SINGLETON(PartyManager)->AddToActiveRoster(0ui64, 0ui64);

	GET_SINGLETON(BattleManager)->SetEncounterID(0ui16);

	return GET_SINGLETON(BattleManager)->OnUserCreate();
}

bool BattleScene::OnUserDestroy() {
	return true;
}

bool BattleScene::OnUserUpdate(float_t _deltaTime) {
	return GET_SINGLETON(BattleManager)->OnUserUpdate(_deltaTime);
}

bool BattleScene::OnUserRender(float_t _deltaTime) {
	return GET_SINGLETON(BattleManager)->OnUserRender(_deltaTime);
}

bool BattleScene::OnBeforeUserUpdate(float_t _deltaTime) {
	return GET_SINGLETON(BattleManager)->OnBeforeUserUpdate(_deltaTime);
}

void BattleScene::OnAfterUserUpdate(float_t _deltaTime) {
	GET_SINGLETON(BattleManager)->OnAfterUserUpdate(_deltaTime);
}