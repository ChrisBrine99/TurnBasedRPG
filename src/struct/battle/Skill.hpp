#ifndef SKILL_HPP
#define SKILL_HPP

#include "../../utility/BattleMacros.hpp"
#include "../../utility/DataMacros.hpp"
#include "../../utility/GeneralMacros.hpp"

#include <array>
#include <cstdint>

class BattleScene;
struct Combatant;

struct Skill {
	std::array<char, SKILL_INTERNAL_NAME_SIZE>
						name;		// The name of the skill.
	uint16_t			id;			// Unique identifier between 0 and 65,535 that can be used to reference the skill's data outside this struct.
	uint8_t				affinity;	// Unique identifier that determines what the skill will do when used against an enemy or on an ally.
	uint8_t				targeting;	// Determines how the skill will affect combatants on the casters side of the field and on the opposite side.
	uint32_t			flags;		// Additional paramaters that can be toggled on or off to enable/disable certain properties for the skill.

public: // Constructor/Destructor Declarations
	Skill();
	Skill(const Skill& _other) = delete;
	~Skill() = default;
};

#endif