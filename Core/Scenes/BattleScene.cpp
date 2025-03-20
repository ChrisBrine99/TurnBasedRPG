#include "BattleScene.hpp"

#include "../Singletons/BattleManager.hpp"
#include "../Singletons/PartyManager.hpp"

BattleScene::BattleScene(uint32_t _index) :
	Scene(_index)
{}

bool BattleScene::OnUserCreate() {
	CALL_SINGLETON_CREATE(BattleManager, OnUserCreate)

	GET_SINGLETON(PartyManager)->AddPartyMember(ID_TEST_PLAYER);
	GET_SINGLETON(PartyManager)->AddToPartyRoster(ID_TEST_PLAYER);
	GET_SINGLETON(PartyManager)->AddToActiveRoster(0ui64, 0ui64);

	GET_SINGLETON(BattleManager)->SetEncounterID(0ui16);

	return true;
}

bool BattleScene::OnUserDestroy() {
	return true;
}

bool BattleScene::OnUserUpdate(float_t _deltaTime) {
	CALL_SINGLETON_UPDATE(BattleManager, OnUserUpdate)
	return true;
}

bool BattleScene::OnUserRender(float_t _deltaTime) {
	CALL_SINGLETON_RENDER(BattleManager)
	return true;
}

bool BattleScene::OnBeforeUserUpdate(float_t _deltaTime) {
	CALL_SINGLETON_UPDATE(BattleManager, OnBeforeUserUpdate)
	return true;
}

bool BattleScene::OnAfterUserUpdate(float_t _deltaTime) {
	CALL_SINGLETON_UPDATE(BattleManager, OnAfterUserUpdate)
	return true;
}