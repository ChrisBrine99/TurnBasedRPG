#ifndef PARTY_MANAGER_HPP
#define PARTY_MANAGER_HPP

#include "../Utils/GeneralMacros.hpp"
#include "../Structs/Characters/PlayerCharacter.hpp"

#include <unordered_map>

class PartyManager {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(PartyManager)
public: // Main Engine Function Declarations
	bool OnUserDestroy();

public: // Publicly Accessible Utility Function Declarations
	void AddPartyMember(uint16_t _characterID);
	void RemovePartyMember(uint16_t _characterID);
	uint16_t GetPartyMemberID(size_t _partyIndex);

	void AddToPartyRoster(uint16_t _characterID);
	void RemoveFromPartyRoster(std::vector<uint16_t>::iterator& _position);

	bool AddToActiveRoster(size_t _activeSlot, size_t _partyIndex);
	bool RemoveFromActiveRoster(uint16_t _characterID);
	PlayerCharacter* GetActiveRosterMember(size_t _activeSlot);

private: // Hidden Member Variable Declarations
	std::unordered_map<uint16_t, PlayerCharacter>	partyMembers;		// Stores and manages ALL party members (Even if they aren't in the party at the moment).
	std::vector<uint16_t>							curPartyRoster;		// Index values that point to currently existing members out of the entire group.
	std::array<uint16_t, PARTY_ACTIVE_MAX_SIZE>		curActiveRoster;	// Similar to the vector above, but only stores the currently active party member indexes.
};

#endif