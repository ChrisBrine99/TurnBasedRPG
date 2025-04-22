#include "Skill.hpp"

Skill::Skill() :
	name('\0'),
	id(SKL_INVALID),
	affinity(AFFINITY_INVALID),
	targeting(TARGET_INVALID),
	flags(0ui16)
{}