#ifndef CHARACTER_MACROS_HPP
#define	CHARACTER_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Various constants for maximum possible values throughout the character's various stats.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define CHR_MAX_LEVEL					100ui8
#define CHR_MAX_STAT_VALUE				100ui8
#define CHR_MAX_HP_AND_MP				999ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Various constants for minimum possible values throughout the character's various stats.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define CHR_MIN_STAT_VALUE				1ui8
#define CHR_MIN_HP_AND_MP				1ui16
#define PCHR_MIN_HP_AND_MP				10ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Makes sense of the numbers that represent the array indices that store each respective character stat.								//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define	CHR_STAT_STRENGTH				0ui64
#define CHR_STAT_MAGIC					1ui64
#define CHR_STAT_ENDURANCE				2ui64
#define CHR_STAT_INTELLIGENCE			3ui64
#define CHR_STAT_AGILITY				4ui64
#define CHR_STAT_CONCENTRATION			5ui64
#define CHR_STAT_LUCK					6ui64
#define CHR_STAT_COUNT					7ui64

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	A specific constant that limits the total number of skills a player character can have access to within a battle. Enemies are		//
//	excempt from this limit.																											//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define PCHR_SKILL_LIMIT				6ui64

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Determines the maximum length of a character's name. The effective includes the null-terminator, and the effective name is how		//
//	many visible characters can be in the name.																							//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define	CHR_INTERNAL_NAME_SIZE			18ui64
#define CHR_EFFECTIVE_NAME_SIZE			17ui64

#endif