#pragma once

#include "ItemData.h"
#ifdef ENABLE_CHEST_DROP
#include <unordered_map>
#endif

class CItemManager : public CSingleton<CItemManager>
{
	public:
		enum EItemDescCol
		{
			ITEMDESC_COL_VNUM,
			ITEMDESC_COL_NAME,						
			ITEMDESC_COL_DESC,
			ITEMDESC_COL_SUMM,
			ITEMDESC_COL_NUM,
		};

		enum EItemScaleCol
		{
			ITEMSCALE_COL_VNUM,
			ITEMSCALE_COL_JOB,
			ITEMSCALE_COL_SEX,
			ITEMSCALE_COL_SCALE_X,
			ITEMSCALE_COL_SCALE_Y,
			ITEMSCALE_COL_SCALE_Z,
			ITEMSCALE_COL_PARTICLE_SCALE,
		};

	public:
		typedef std::map<DWORD, CItemData*> TItemMap;
		typedef std::map<std::string, CItemData*> TItemNameMap;

#ifdef INGAME_WIKI
	public:
		typedef std::vector<CItemData*> TItemVec;
		typedef std::vector<_wuint32> TItemNumVec;

	public:
		void WikiAddVnumToBlacklist(_wuint32 vnum)
		{
			auto it = m_ItemMap.find(vnum);
			if (it != m_ItemMap.end())
				it->second->SetBlacklisted(true);
		};

		TItemNumVec* WikiGetLastItems()
		{
			return &m_tempItemVec;
		}

		bool								CanLoadWikiItem(_wuint32 dwVnum);
		_wuint32							GetWikiItemStartRefineVnum(_wuint32 dwVnum);
		std::string							GetWikiItemBaseRefineName(_wuint32 dwVnum);
		size_t								WikiLoadClassItems(_wunsignedchar classType, _wuint32 raceFilter);
		std::tuple<const char*, _wint32>	SelectByNamePart(const char* namePart);

	protected:
		TItemNumVec m_tempItemVec;

	private:
		bool IsFilteredAntiflag(CItemData* itemData, _wuint32 raceFilter);
#endif

	public:
		CItemManager();
		virtual ~CItemManager();
		
		void			Destroy();

		BOOL			SelectItemData(DWORD dwIndex);
		CItemData *		GetSelectedItemDataPointer();

		int				CanIncrSelectedItemRefineLevel();

		BOOL			GetItemDataPointer(DWORD dwItemID, CItemData ** ppItemData);

		bool			LoadItemName(const char* c_szFileName);
		bool			LoadItemDesc(const char* c_szFileName);
		bool			LoadItemList(const char* c_szFileName);
		bool			LoadItemTable(const char* c_szFileName);
#ifdef ENABLE_SHINING_SYSTEM
		bool            LoadShiningTable(const char* szShiningTable);
#endif
		bool			LoadItemScale(const char* szItemScale);
		CItemData *		MakeItemData(DWORD dwIndex);
#ifdef ENABLE_CHEST_DROP
		struct SDropItemInfo
		{
			DWORD	dwDropVnum;
			int		iCount;
		};

		using	TChestDropItemInfoVec = std::vector<SDropItemInfo>;
		using	TChestDropItemInfoMap = std::unordered_map<DWORD, TChestDropItemInfoVec>;

		bool	LoadChestDropInfo(const char* c_szFileName);
		const TChestDropItemInfoVec* GetItemDropInfoVec(const DWORD dwVnum) const;
#endif

	protected:
		TItemMap m_ItemMap;
		std::vector<CItemData*>  m_vec_ItemRange;
		CItemData * m_pSelectedItemData;
#ifdef ENABLE_CHEST_DROP
		TChestDropItemInfoMap m_ItemDropInfoMap;
#endif
};
