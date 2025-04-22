#include "PassiveSkill.hpp"

PassiveSkill::PassiveSkill() : 
	Skill(),
	maxHpBonus(0i16),
	maxMpBonus(0i16),
	maxHpMultiplier(0ui8),
	maxMpMultiplier(0ui8),
	affinityFlags(0ui16),
	affinityModifier(AFFINITY_INVALID),
	buffDurationBonus(0ui8),
	critChanceBonus(0ui8),
	critDamageBonus(0ui8),
	resistOverwrite(EFFECT_NORMAL),
	useSkillID(SKL_INVALID)
{}