#include "ActiveSkill.hpp"

ActiveSkill::ActiveSkill() : 
	Skill(),
	basePower(0ui16),
	accuracy(0ui8),
	hpCost(0ui16),
	mpCost(0ui16),
	hitCount(0ui8),
	critChance(0ui8),
	critBonus(0ui8),
	recoilPower(0ui8),
	addedEffects({ AILMENT_INVALID, AILMENT_INVALID, AILMENT_INVALID, AILMENT_INVALID }),
	effectChance(0ui8),
	buffAmount(0x36DBui16),
	buffDuration(0ui8),
	dmgMultiplier(0ui8),
	healPower(0ui16)
{}