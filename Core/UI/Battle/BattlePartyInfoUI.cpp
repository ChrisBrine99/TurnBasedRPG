#include "BattlePartyInfoUI.hpp"

#include "../../Singletons/PartyManager.hpp"
#include "../../Utils/BattleMacros.hpp"

BattlePartyInfoUI::BattlePartyInfoUI() :
	flags(0u),
	uiElements()
{ // Reserve enough memory to contain three elements since that is the max number of party members that can participate in a battle at once.
	uiElements.reserve(BATTLE_MAX_PARTY_SIZE);
}

bool BattlePartyInfoUI::OnUserCreate() {
	for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++)
		uiElements.push_back(PartyMemberUI()); // Initialize all three instaces upon creation of the UI.
	return true;
}

bool BattlePartyInfoUI::OnUserRender(float _deltaTime) {
	for (PartyMemberUI& _element : uiElements) {
		if (!(_element.flags & FLAG_PMINFO_VISIBLE))
			continue;

	}
	return true;
}

void BattlePartyInfoUI::ActivateElement(size_t _activeSlot) {
	if (_activeSlot >= uiElements.size() || FLAG_IS_PMINFO_OCCUPIED(_activeSlot))
		return;

	PartyManager* _manager = GET_SINGLETON(PartyManager);
	PlayerCharacter* _character	= _manager->GetActiveRosterMember(_activeSlot);
	if (_character == nullptr) // The required index return nullptr; don't initialize the element.
		return;

	PartyMemberUI& _uiElement	= uiElements[_activeSlot];
	_uiElement.flags			= FLAG_PMINFO_OCCUPIED;
}

void BattlePartyInfoUI::DeactivateElement(size_t _uiSlot) {

}