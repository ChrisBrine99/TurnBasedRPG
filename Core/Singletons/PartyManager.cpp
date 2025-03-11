#include "PartyManager.hpp"
INIT_SINGLETON_CPP(PartyManager)

#include "DataManager.hpp"

PartyManager::PartyManager() :
	partyMembers(),
	curPartyRoster(),
	curActiveRoster({ ID_INVALID, ID_INVALID, ID_INVALID })
{ // Reserve enough memory to store data within the vector representing the current party members.
	curPartyRoster.reserve(PARTY_ROSTER_MAX_SIZE);
}

bool PartyManager::OnUserDestroy() {
	partyMembers.clear();
	curPartyRoster.clear();
	curPartyRoster.shrink_to_fit();

	return true;
}

void PartyManager::AddPartyMember(uint16_t _characterID) {
	if (_characterID < ID_BOUNDARY || partyMembers.find(_characterID) != partyMembers.end())
		return;

	PlayerCharacter* _character = (PlayerCharacter*)GET_SINGLETON(DataManager)->GetCharacter(_characterID);
	if (_character == nullptr)
		return;

	PlayerCharacter _pCharacter(*_character);
	partyMembers[_characterID] = _pCharacter;
}

void PartyManager::RemovePartyMember(uint16_t _characterID) {
	if (partyMembers.find(_characterID) == partyMembers.end())
		return;
	partyMembers.erase(_characterID);

	auto _partyPosition = std::find(curPartyRoster.begin(), curPartyRoster.end(), _characterID);
	RemoveFromPartyRoster(_partyPosition);
	RemoveFromActiveRoster(_characterID);
}

void PartyManager::AddToPartyRoster(uint16_t _characterID) {
	if (partyMembers.find(_characterID) == partyMembers.end() || 
			std::find(curPartyRoster.begin(), curPartyRoster.end(), _characterID) != curPartyRoster.end())
		return;
	curPartyRoster.push_back(_characterID);
}

void PartyManager::RemoveFromPartyRoster(std::vector<uint16_t>::iterator& _position) {
	if (_position == curPartyRoster.end())
		return;
	curPartyRoster.erase(_position);
}

bool PartyManager::AddToActiveRoster(size_t _activeSlot, size_t _partyIndex) {
	if (_activeSlot >= PARTY_ACTIVE_MAX_SIZE || _partyIndex >= curPartyRoster.size())
		return false;

	uint16_t _characterID = curPartyRoster[_partyIndex];
	RemoveFromActiveRoster(_characterID);

	curActiveRoster[_activeSlot] = _characterID;
	return true;
}

bool PartyManager::RemoveFromActiveRoster(uint16_t _characterID) {
	for (size_t i = 0ui64; i < PARTY_ACTIVE_MAX_SIZE; i++) {
		if (curActiveRoster[i] == _characterID) {
			curActiveRoster[i] = ID_INVALID;
			return true;
		}
	}
	return false;
}