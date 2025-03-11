#ifndef CHARACTER_MACROS_HPP
#define	CHARACTER_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Various constants for maximum possible values throughout the character's various stats.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define MAXIMUM_LEVEL					100ui8
#define MAXIMUM_STAT_VALUE				100ui8
#define MAXIMUM_HP_AND_MP				999ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Various constants for minimum possible values throughout the character's various stats.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define MINIMUM_STAT_VALUE				1ui8
#define MINIMUM_HP_AND_MP				1ui16
#define MINIMUM_PLAYER_HP_AND_MP		10ui16

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Makes sense of the numbers that represent the array indices that store each respective character stat.								//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define	STAT_STRENGTH					0ui64
#define STAT_MAGIC						1ui64
#define STAT_ENDURANCE					2ui64
#define STAT_INTELLIGENCE				3ui64
#define STAT_AGILITY					4ui64
#define STAT_CONCENTRATION				5ui64
#define STAT_LUCK						6ui64
#define STAT_COUNT						7ui64

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	A specific constant that limits the total number of skills a player character can have access to within a battle. Enemies are		//
//	excempt from this limit.																											//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define PLAYER_SKILL_LIMIT				6ui64

#endif