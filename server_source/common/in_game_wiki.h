#define __IS_SERVER_WIKI_INCLUDE__

#ifndef __IS_SERVER_WIKI_INCLUDE__
#pragma once
#endif

#ifndef __INC_IN_GAME_WIKI_H__
#define __INC_IN_GAME_WIKI_H__

#pragma pack(1)

typedef float _wfloat;

typedef int _wint32;
typedef unsigned long long _wuint64;

typedef unsigned short _wunsignedshort;
typedef WORD _wGC_PACKAGE_SIZE;
typedef DWORD _wuint32;

typedef BYTE _wunsignedchar;
typedef BYTE _wHEADER_TYPE;

namespace CommonWikiData
{
	static const _wint32 MAX_REFINE_COUNT = 9;
	static const _wint32 REFINE_MATERIAL_MAX_NUM = 5;
	
	typedef struct SRefineMaterialNew
	{
		~SRefineMaterialNew() = default;
		SRefineMaterialNew() {
			vnum = 0;
			count = 0;
		}
		
		_wuint32 vnum;
		_wint32 count;
	} TRefineMaterialNew;
	
	typedef struct SWikiChestInfo
	{
		~SWikiChestInfo() = default;
		SWikiChestInfo(_wuint32 r_vnum = 0, _wuint32 r_count = 0) {
			vnum = r_vnum;
			count = r_count;
		}
		
		_wuint32 vnum;
		_wint32 count;
	} TWikiChestInfo;
	
	typedef struct SWikiMobDropInfo
	{
		~SWikiMobDropInfo() = default;
		SWikiMobDropInfo(_wuint32 r_vnum = 0, _wuint32 r_count = 0) {
			vnum = r_vnum;
			count = r_count;
		}
		
		_wuint32 vnum;
		_wint32 count;
	} TWikiMobDropInfo;
	
	typedef struct SWikiRefineInfo
	{
		~SWikiRefineInfo() = default;
		SWikiRefineInfo() {
			index = 0;
			memset(&materials, 0, sizeof(materials));
			mat_count = 0;
			price = 0;
		}
		
		_wint32 index;
		TRefineMaterialNew materials[REFINE_MATERIAL_MAX_NUM];
		_wuint32 mat_count;
		_wint32 price;
	} TWikiRefineInfo;
	
	typedef struct SWikiItemOriginInfo
	{
		~SWikiItemOriginInfo() = default;
		SWikiItemOriginInfo(_wuint32 r_vnum = 0, bool r_is_mob = false) {
			vnum = r_vnum;
			is_mob = r_is_mob;
		}
		
		void set_vnum(_wuint32 r_vnum) { vnum = r_vnum; }
		void set_is_mob(bool flag) { is_mob = flag; }
		
		_wuint32 vnum;
		bool is_mob;
	} TWikiItemOriginInfo;
	
	typedef struct SWikiInfoTable
	{
		~SWikiInfoTable() = default;
		SWikiInfoTable() {
			is_common = false;
			refine_infos_count = 0;
			chest_info_count = 0;
			origin_vnum = 0;
		}
		
		bool is_common;
		_wint32 refine_infos_count;
		_wint32 chest_info_count;
		_wuint32 origin_vnum;
	} TWikiInfoTable;
}

namespace InGameWiki
{
	enum HCommunicationsPacketsCG
	{
		HEADER_CG_WIKI = 210,
	};

	enum HCommunicationsPacketsGC
	{
		HEADER_GC_WIKI = 213,
	};
	
	enum SHCommunicationsPacketsGC
	{
		LOAD_WIKI_ITEM,
		LOAD_WIKI_MOB,
	};
	
	using namespace CommonWikiData;
	
	typedef struct SCGWikiPacket
	{
		~SCGWikiPacket() = default;
		SCGWikiPacket() {
			bHeader = HEADER_CG_WIKI;
			is_mob = false;
			vnum = 0;
			ret_id = 0;
		}

		_wHEADER_TYPE bHeader;
		bool is_mob;
		_wuint32 vnum;
		_wuint64 ret_id;
	} TCGWikiPacket;
	
	typedef struct SGCWikiPacket
	{
		~SGCWikiPacket() = default;
		SGCWikiPacket() {
			bHeader = HEADER_GC_WIKI;
			size = 0;
			bSubHeader = LOAD_WIKI_ITEM;
		}
		
		void increment_data_size(_wGC_PACKAGE_SIZE r_data_size) { size += r_data_size; }
		void set_data_type(_wHEADER_TYPE r_dataType) { bSubHeader = r_dataType; }
		bool is_data_type(_wHEADER_TYPE r_dataType) { return bSubHeader == r_dataType; }
		
		_wHEADER_TYPE bHeader;
		_wGC_PACKAGE_SIZE size;
		_wHEADER_TYPE bSubHeader;
	} TGCWikiPacket;
	
	typedef struct SGCItemWikiPacket
	{
		~SGCItemWikiPacket() = default;
		SGCItemWikiPacket() = default;
		
		void mutable_wiki_info(TWikiInfoTable r_wif) { wiki_info = r_wif; }
		void set_origin_infos_count(_wint32 elem_count) { origin_infos_count = elem_count; }
		void set_vnum(_wuint32 r_vnum) { vnum = r_vnum; }
		void set_ret_id(_wuint64 r_ret_id) { ret_id = r_ret_id; }
		
		TWikiInfoTable wiki_info;
		_wint32 origin_infos_count;
		_wuint32 vnum;
		_wuint64 ret_id;
	} TGCItemWikiPacket;
	
	typedef struct SGCMobWikiPacket
	{
		~SGCMobWikiPacket() = default;
		SGCMobWikiPacket() {
			vnum = 0;
			ret_id = 0;
		}
		
		void set_drop_info_count(_wint32 r_drop_info_count) { drop_info_count = r_drop_info_count; }
		void set_vnum(_wuint32 r_vnum) { vnum = r_vnum; }
		void set_ret_id(_wuint64 r_ret_id) { ret_id = r_ret_id; }
		
		_wint32 drop_info_count;
		_wuint32 vnum;
		_wuint64 ret_id;
	} TGCMobWikiPacket;
}

#pragma pack()
#endif // __INC_IN_GAME_WIKI_H__
