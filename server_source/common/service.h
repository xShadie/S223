#pragma once

enum 
{
	PARTY_CAN_JOIN_MAX_LEVEL	= 30,
	DEFAULT_ATTACK_SPEED		= 100,
	DEFAULT_MOVE_SPEED			= 200,
	MOVEMENT_SPEED_LIMIT		= 250,
	MAX_MEMBER_INTRO_PARTY		= 8,
	MAX_ITEM_STACK				= 5000,
};

//#define _IMPROVED_PACKET_ENCRYPTION_
//#define ENABLE_MIRRORING_PROTO
//#define ENABLE_SEQUENCE_SYSTEM
//#define ENABLE_WOLFMAN
//#define ENABLE_OPEN_MULTIPLE

#define ENABLE_UDP_BLOCK
#define CLIENT_VERSION "20181010"
#define LSS_SECURITY_KEY "1234abcd5678efgh"

#define ENABLE_TARGET_INFO
#define ENABLE_MASTER_SKILLBOOK_NO_STEPS
#define ITEM_CHECKINOUT_UPDATE
#define ENABLE_INSTANT_5_BONUS
#define ENABLE_XOSHIRO
#define ENABLE_PERFORMANCE_TEST
//#define ENABLE_NEW_CHARSELECT // sql add part_weapon
#define ENABLE_MULTILANGUAGE
#ifdef ENABLE_MULTILANGUAGE
#define ENABLE_LANGUAGE_FLAGS
#endif
//#define ENABLE_ITEMSHOP
#define ENABLE_ANTI_EXP
#define ENABLE_NEW_TYPE_OF_POTION //Permanente Buffitems an-/ausschalten
#define DROP_LEVEL_LIMIT_RANGE
#define __INGAME_WIKI__
#define KEPP_SKILL_AT_WARP
#define ENABLE_VOTE_FOR_BONUS
//#define ENABLE_MOB_MOVE_SPEED
//#define	ENABLE_QUEST_RENEWAL //need client parth
