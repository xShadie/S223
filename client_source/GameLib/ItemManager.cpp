#include "StdAfx.h"
#include "../eterPack/EterPackManager.h"
#include "../eterLib/ResourceManager.h"
#include "ItemManager.h"

static DWORD s_adwItemProtoKey[4] =
{
	173217,
	72619434,
	408587239,
	27973291
};

BOOL CItemManager::SelectItemData(DWORD dwIndex)
{
	TItemMap::iterator f = m_ItemMap.find(dwIndex);

	if (m_ItemMap.end() == f)
	{
		int n = m_vec_ItemRange.size();
		for (int i = 0; i < n; i++)
		{
			CItemData * p = m_vec_ItemRange[i];
			const CItemData::TItemTable * pTable = p->GetTable(); 
			if ((pTable->dwVnum < dwIndex) &&
				dwIndex < (pTable->dwVnum + pTable->dwVnumRange))
			{
				m_pSelectedItemData = p;
				return TRUE;
			}
		}
		Tracef(" CItemManager::SelectItemData - FIND ERROR [%d]\n", dwIndex);
		return FALSE;
	}

	m_pSelectedItemData = f->second;

	return TRUE;
}

CItemData * CItemManager::GetSelectedItemDataPointer()
{
	return m_pSelectedItemData;
}

int CItemManager::CanIncrSelectedItemRefineLevel()
{
	return 0;
}

BOOL CItemManager::GetItemDataPointer(DWORD dwItemID, CItemData ** ppItemData)
{
	if (0 == dwItemID)
		return FALSE;

	TItemMap::iterator f = m_ItemMap.find(dwItemID);

	if (m_ItemMap.end() == f)
	{
		int n = m_vec_ItemRange.size();
		for (int i = 0; i < n; i++)
		{
			CItemData * p = m_vec_ItemRange[i];
			const CItemData::TItemTable * pTable = p->GetTable(); 
			if ((pTable->dwVnum < dwItemID) &&
				dwItemID < (pTable->dwVnum + pTable->dwVnumRange))
			{
				*ppItemData = p;
				return TRUE;
			}
		}
		Tracef(" CItemManager::GetItemDataPointer - FIND ERROR [%d]\n", dwItemID);
		return FALSE;
	}

	*ppItemData = f->second;

	return TRUE;
}

CItemData * CItemManager::MakeItemData(DWORD dwIndex)
{
	TItemMap::iterator f = m_ItemMap.find(dwIndex);

	if (m_ItemMap.end() == f)
	{
		CItemData * pItemData = CItemData::New();

		m_ItemMap.insert(TItemMap::value_type(dwIndex, pItemData));		

		return pItemData;
	}

	return f->second;
}

bool CItemManager::LoadItemList(const char * c_szFileName)
{
	CMappedFile File;
	LPCVOID pData;

	if (!CEterPackManager::Instance().Get(File, c_szFileName, &pData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);

	CTokenVector TokenVector;
    for (DWORD i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLine(i, &TokenVector, "\t"))
			continue;

		if (!(TokenVector.size() == 3 || TokenVector.size() == 4))
		{
			TraceError(" CItemManager::LoadItemList(%s) - StrangeLine in %d\n", c_szFileName, i);
			continue;
		}

		const std::string & c_rstrID = TokenVector[0];
		const std::string & c_rstrIcon = TokenVector[2];

		DWORD dwItemVNum=atoi(c_rstrID.c_str());

		CItemData * pItemData = MakeItemData(dwItemVNum);

		extern BOOL USE_VIETNAM_CONVERT_WEAPON_VNUM;
		if (USE_VIETNAM_CONVERT_WEAPON_VNUM)
		{
			extern DWORD Vietnam_ConvertWeaponVnum(DWORD vnum);
			DWORD dwMildItemVnum = Vietnam_ConvertWeaponVnum(dwItemVNum);
			if (dwMildItemVnum == dwItemVNum)
			{
				if (4 == TokenVector.size())
				{
					const std::string & c_rstrModelFileName = TokenVector[3];
					pItemData->SetDefaultItemData(c_rstrIcon.c_str(), c_rstrModelFileName.c_str());
				}
				else
				{
					pItemData->SetDefaultItemData(c_rstrIcon.c_str());
				}
			}
			else
			{
				DWORD dwMildBaseVnum = dwMildItemVnum / 10 * 10;
				char szMildIconPath[MAX_PATH];				
				sprintf(szMildIconPath, "icon/item/%.5d.tga", dwMildBaseVnum);
				if (4 == TokenVector.size())
				{
					char szMildModelPath[MAX_PATH];
					sprintf(szMildModelPath, "d:/ymir work/item/weapon/%.5d.gr2", dwMildBaseVnum);	
					pItemData->SetDefaultItemData(szMildIconPath, szMildModelPath);
				}
				else
				{
					pItemData->SetDefaultItemData(szMildIconPath);
				}
			}
		}
		else
		{
			if (4 == TokenVector.size())
			{
				const std::string & c_rstrModelFileName = TokenVector[3];
				pItemData->SetDefaultItemData(c_rstrIcon.c_str(), c_rstrModelFileName.c_str());
			}
			else
			{
				pItemData->SetDefaultItemData(c_rstrIcon.c_str());
			}
		}
	}

	return true;
}

const std::string& __SnapString(const std::string& c_rstSrc, std::string& rstTemp)
{
	UINT uSrcLen=c_rstSrc.length();
	if (uSrcLen<2)
		return c_rstSrc;

	if (c_rstSrc[0]!='"')
		return c_rstSrc;

	UINT uLeftCut=1;
	
	UINT uRightCut=uSrcLen;
	if (c_rstSrc[uSrcLen-1]=='"')
		uRightCut=uSrcLen-1;	

	rstTemp=c_rstSrc.substr(uLeftCut, uRightCut-uLeftCut);
	return rstTemp;
}

bool CItemManager::LoadItemDesc(const char* c_szFileName)
{
	const VOID* pvData;
	CMappedFile kFile;
	if (!CEterPackManager::Instance().Get(kFile, c_szFileName, &pvData))
	{
		Tracenf("CItemManager::LoadItemDesc(c_szFileName=%s) - Load Error", c_szFileName);
		return false;
	}

	CMemoryTextFileLoader kTextFileLoader;
	kTextFileLoader.Bind(kFile.Size(), pvData);

	std::string stTemp;

	CTokenVector kTokenVector;
	for (DWORD i = 0; i < kTextFileLoader.GetLineCount(); ++i)
	{
		if (!kTextFileLoader.SplitLineByTab(i, &kTokenVector))
			continue;

		while (kTokenVector.size()<ITEMDESC_COL_NUM)
			kTokenVector.push_back("");

		DWORD dwVnum=atoi(kTokenVector[ITEMDESC_COL_VNUM].c_str());
		const std::string& c_rstDesc=kTokenVector[ITEMDESC_COL_DESC];
		const std::string& c_rstSumm=kTokenVector[ITEMDESC_COL_SUMM];
		TItemMap::iterator f = m_ItemMap.find(dwVnum);
		if (m_ItemMap.end() == f)
			continue;

		CItemData* pkItemDataFind = f->second;

		pkItemDataFind->SetDescription(__SnapString(c_rstDesc, stTemp));
		pkItemDataFind->SetSummary(__SnapString(c_rstSumm, stTemp));
	}
	return true;
}

bool CItemManager::LoadItemName(const char* c_szFileName)
{
	const VOID* pvData;
	CMappedFile kFile;
	if (!CEterPackManager::Instance().Get(kFile, c_szFileName, &pvData))
	{
		Tracenf("CItemManager::LoadItemName(c_szFileName=%s) - Load Error", c_szFileName);
		return false;
	}

	CMemoryTextFileLoader kTextFileLoader;
	kTextFileLoader.Bind(kFile.Size(), pvData);

	CTokenVector kTokenVector;
	for (DWORD i = 0; i < kTextFileLoader.GetLineCount(); ++i)
	{
		if (!kTextFileLoader.SplitLineByTab(i, &kTokenVector))
			continue;

		auto it = m_ItemMap.find(atoi(kTokenVector[0].c_str()));
		if (m_ItemMap.end() != it)
		{
			it->second->SetName(kTokenVector[1].c_str());
		}
	}

	return true;
}

DWORD GetHashCode( const char* pString )
{
	   unsigned long i,len;
	   unsigned long ch;
	   unsigned long result;
	   
	   len     = strlen( pString );
	   result = 5381;
	   for( i=0; i<len; i++ )
	   {
	   	   ch = (unsigned long)pString[i];
	   	   result = ((result<< 5) + result) + ch;
	   }	  

	   return result;
}

bool CItemManager::LoadItemTable(const char* c_szFileName)
{	
	CMappedFile file;
	LPCVOID pvData;

	if (!CEterPackManager::Instance().Get(file, c_szFileName, &pvData))
		return false;

	DWORD dwFourCC, dwElements, dwDataSize;
	DWORD dwVersion=0;
	DWORD dwStride=0;

	file.Read(&dwFourCC, sizeof(DWORD));

	if (dwFourCC == MAKEFOURCC('M', 'I', 'P', 'X'))
	{
		file.Read(&dwVersion, sizeof(DWORD));
		file.Read(&dwStride, sizeof(DWORD));
	
		if (dwVersion != 1)
		{
			TraceError("CPythonItem::LoadItemTable: invalid item_proto[%s] VERSION[%d]", c_szFileName, dwVersion);
			return false;
		}
		if (dwStride != sizeof(CItemData::TItemTable))
		{
			TraceError("CPythonItem::LoadItemTable: invalid item_proto[%s] STRIDE[%d] != sizeof(SItemTable)", 
				c_szFileName, dwStride, sizeof(CItemData::TItemTable));
			return false;
		}
	}
	else if (dwFourCC != MAKEFOURCC('M', 'I', 'P', 'T'))
	{
		TraceError("CPythonItem::LoadItemTable: invalid item proto type %s", c_szFileName);
		return false;
	}

	file.Read(&dwElements, sizeof(DWORD));
	file.Read(&dwDataSize, sizeof(DWORD));

	BYTE * pbData = new BYTE[dwDataSize];
	file.Read(pbData, dwDataSize);

	CLZObject zObj;

	if (!CLZO::Instance().Decompress(zObj, pbData, s_adwItemProtoKey))
	{
		delete [] pbData;
		return false;
	}

	char szName[64+1];
	
	CItemData::TItemTable * table = (CItemData::TItemTable *) zObj.GetBuffer();
	std::map<DWORD,DWORD> itemNameMap;

	for (DWORD i = 0; i < dwElements; ++i, ++table)
	{
		CItemData * pItemData;
		DWORD dwVnum = table->dwVnum;

		TItemMap::iterator f = m_ItemMap.find(dwVnum);
		if (m_ItemMap.end() == f)
		{
			_snprintf(szName, sizeof(szName), "icon/item/%05d.tga", dwVnum);
#ifdef INGAME_WIKI
			pItemData = CItemData::New();
#endif

			if (CResourceManager::Instance().IsFileExist(szName) == false)
			{
#ifdef INGAME_WIKI
				pItemData->ValidateImage(false);
#endif

				std::map<DWORD, DWORD>::iterator itVnum = itemNameMap.find(GetHashCode(table->szName));
				
				if (itVnum != itemNameMap.end())
					_snprintf(szName, sizeof(szName), "icon/item/%05d.tga", itVnum->second);
				else
					_snprintf(szName, sizeof(szName), "icon/item/%05d.tga", dwVnum-dwVnum % 10);

				if (CResourceManager::Instance().IsFileExist(szName) == false)
				{
					#ifdef _DEBUG
					TraceError("%16s(#%-5d) cannot find icon file. setting to default.", table->szName, dwVnum);
					#endif
					const DWORD EmptyBowl = 27995;
					_snprintf(szName, sizeof(szName), "icon/item/%05d.tga", EmptyBowl);
				}
			}
				
			pItemData = CItemData::New();

			pItemData->SetDefaultItemData(szName);
			m_ItemMap.insert(TItemMap::value_type(dwVnum, pItemData));

#ifdef INGAME_WIKI
			pItemData->SetItemTableData(table);
			if (!CResourceManager::Instance().IsFileExist(pItemData->GetIconFileName().c_str()))
				pItemData->ValidateImage(false);
#endif
		}
		else
		{
			pItemData = f->second;
#ifdef INGAME_WIKI
			pItemData->SetItemTableData(table);
#endif
		}

		if (itemNameMap.find(GetHashCode(table->szName)) == itemNameMap.end())
			itemNameMap.insert(std::map<DWORD,DWORD>::value_type(GetHashCode(table->szName),table->dwVnum));

		pItemData->SetItemTableData(table);
		if (0 != table->dwVnumRange)
		{
			m_vec_ItemRange.push_back(pItemData);
		}
	}

	delete [] pbData;
	return true;
}

#ifdef ENABLE_SHINING_SYSTEM
bool CItemManager::LoadShiningTable(const char* szShiningTable)
{
	CMappedFile File;
	LPCVOID pData;
	if (!CEterPackManager::Instance().Get(File, szShiningTable, &pData))
		return false;
	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);
	CTokenVector TokenVector;
	for (DWORD i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLine(i, &TokenVector, "\t"))
			continue;
		if (TokenVector.size() > (1 + CItemData::ITEM_SHINING_MAX_COUNT))
		{
			TraceError("CItemManager::LoadShiningTable(%s) - LoadShiningTable in %d\n - RowSize: %d MaxRowSize: %d", szShiningTable, i, TokenVector.size(), CItemData::ITEM_SHINING_MAX_COUNT);
		}
		const std::string& c_rstrID = TokenVector[0];
		int pos = c_rstrID.find("~");
		if (std::string::npos == pos)
		{
			DWORD dwItemVNum = atoi(c_rstrID.c_str());
			CItemData* pItemData = MakeItemData(dwItemVNum);
			if (pItemData)
			{
				for (BYTE i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
				{
					if (i < (TokenVector.size() - 1))
					{
						const std::string& c_rstrEffectPath = TokenVector[i + 1];
						pItemData->SetItemShiningTableData(i, c_rstrEffectPath.c_str());
					}
					else
					{
						pItemData->SetItemShiningTableData(i, "");
					}
				}
			}
		}
		else
		{
			std::string s_start_vnum(c_rstrID.substr(0, pos));
			std::string s_end_vnum(c_rstrID.substr(pos + 1));
			int start_vnum = atoi(s_start_vnum.c_str());
			int end_vnum = atoi(s_end_vnum.c_str());
			DWORD vnum = start_vnum;
			while (vnum <= end_vnum)
			{
				CItemData* pItemData = MakeItemData(vnum);
				if (pItemData)
				{
					for (BYTE i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
					{
						if (i < (TokenVector.size() - 1))
						{
							const std::string& c_rstrEffectPath = TokenVector[i + 1];
							pItemData->SetItemShiningTableData(i, c_rstrEffectPath.c_str());
						}
						else
						{
							pItemData->SetItemShiningTableData(i, "");
						}
					}
				}
				++vnum;
			}
		}
	}
	return true;
}
#endif

bool CItemManager::LoadItemScale(const char* szItemScale)
{
	CMappedFile File;
	LPCVOID pData;
	if (!CEterPackManager::Instance().Get(File, szItemScale, &pData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);

	CTokenVector TokenVector;
	for (DWORD i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLine(i, &TokenVector, "\t"))
			continue;

		if (!(TokenVector.size() == 6 || TokenVector.size() == 7))
		{
			TraceError(" CItemManager::LoadItemScale(%s) - Error on line %d\n", szItemScale, i);
			continue;
		}

		const std::string& c_rstrID = TokenVector[ITEMSCALE_COL_VNUM];
		const std::string& c_rstrJob = TokenVector[ITEMSCALE_COL_JOB];
		const std::string& c_rstrSex = TokenVector[ITEMSCALE_COL_SEX];
		const std::string& c_rstrScaleX = TokenVector[ITEMSCALE_COL_SCALE_X];
		const std::string& c_rstrScaleY = TokenVector[ITEMSCALE_COL_SCALE_Y];
		const std::string& c_rstrScaleZ = TokenVector[ITEMSCALE_COL_SCALE_Z];

		DWORD dwItemVnum = atoi(c_rstrID.c_str());
		BYTE bJob = 0;
		if (!strcmp(c_rstrJob.c_str(), "JOB_WARRIOR")) bJob = NRaceData::JOB_WARRIOR;
		if (!strcmp(c_rstrJob.c_str(), "JOB_ASSASSIN")) bJob = NRaceData::JOB_ASSASSIN;
		if (!strcmp(c_rstrJob.c_str(), "JOB_SURA")) bJob = NRaceData::JOB_SURA;
		if (!strcmp(c_rstrJob.c_str(), "JOB_SHAMAN")) bJob = NRaceData::JOB_SHAMAN;
#ifdef ENABLE_WOLFMAN
		if (!strcmp(c_rstrJob.c_str(), "JOB_WOLFMAN")) bJob = NRaceData::JOB_WOLFMAN;
#endif
		BYTE bySex = c_rstrSex[0] == 'M';

		float fScaleX = atof(c_rstrScaleX.c_str()) * 0.01f;
		float fScaleY = atof(c_rstrScaleY.c_str()) * 0.01f;
		float fScaleZ = atof(c_rstrScaleZ.c_str()) * 0.01f;
		float fParticleScale = 1.0f;
		if (TokenVector.size() == 7)
		{
			const std::string& c_rstrParticleScale = TokenVector[ITEMSCALE_COL_PARTICLE_SCALE];
			fParticleScale = atof(c_rstrParticleScale.c_str());
		}

		CItemData* pItemData = MakeItemData(dwItemVnum);
		BYTE bGradeMax = 5;
		if (pItemData->GetType() == CItemData::ITEM_TYPE_COSTUME)
			bGradeMax = 6;

		for (BYTE i = 0; i < bGradeMax; ++i)
		{
			pItemData = MakeItemData(dwItemVnum + i);
			if (pItemData)
				pItemData->SetItemTableScaleData(bJob, bySex, fScaleX, fScaleY, fScaleZ, fParticleScale);
		}
	}

	return true;
}

void CItemManager::Destroy()
{
	TItemMap::iterator i;
	for (i=m_ItemMap.begin(); i!=m_ItemMap.end(); ++i)
		CItemData::Delete(i->second);

	m_ItemMap.clear();
#ifdef INGAME_WIKI
	m_tempItemVec.clear();
#endif
}

#ifdef INGAME_WIKI
bool CItemManager::CanLoadWikiItem(DWORD dwVnum)
{
	DWORD StartRefineVnum = GetWikiItemStartRefineVnum(dwVnum);

	if (StartRefineVnum != dwVnum)
		return false;

	if (StartRefineVnum % 10 != 0)
		return false;

	CItemData* tbl = nullptr;
	if (!GetItemDataPointer(StartRefineVnum, &tbl))
		return false;

	return true;
}

DWORD CItemManager::GetWikiItemStartRefineVnum(DWORD dwVnum)
{
	auto baseItemName = GetWikiItemBaseRefineName(dwVnum);
	if (!baseItemName.size())
		return 0;

	DWORD manage_vnum = dwVnum;
	while (!(strcmp(baseItemName.c_str(), GetWikiItemBaseRefineName(manage_vnum).c_str())))
		--manage_vnum;

	return (manage_vnum + 1);
}

std::string CItemManager::GetWikiItemBaseRefineName(DWORD dwVnum)
{
	CItemData* tbl = nullptr;
	if (!GetItemDataPointer(dwVnum, &tbl))
		return "";

	auto* p = const_cast<char*>(strrchr(tbl->GetName(), '+'));
	if (!p)
		return "";

	std::string sFirstItemName(tbl->GetName(),
		(tbl->GetName() + (p - tbl->GetName())));

	return sFirstItemName;
}

bool CItemManager::IsFilteredAntiflag(CItemData* itemData, DWORD raceFilter)
{
	if (raceFilter != 0)
	{
		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_SHAMAN) && raceFilter & CItemData::ITEM_ANTIFLAG_SHAMAN)
			return false;

		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_SURA) && raceFilter & CItemData::ITEM_ANTIFLAG_SURA)
			return false;

		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_ASSASSIN) && raceFilter & CItemData::ITEM_ANTIFLAG_ASSASSIN)
			return false;

		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_WARRIOR) && raceFilter & CItemData::ITEM_ANTIFLAG_WARRIOR)
			return false;

#ifdef INGAME_WIKI_WOLFMAN
		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_WOLFMAN) && raceFilter & CItemData::ITEM_ANTIFLAG_WOLFMAN)
			return false;
#endif
	}

	return true;
}

size_t CItemManager::WikiLoadClassItems(_wunsignedchar classType, DWORD raceFilter)
{
	m_tempItemVec.clear();

	for (TItemMap::iterator it = m_ItemMap.begin(); it != m_ItemMap.end(); ++it)
	{
		if (!it->second->IsValidImage() || it->first < 10 || it->second->IsBlacklisted())
			continue;

		bool _can_load = CanLoadWikiItem(it->first);

		switch (classType)
		{
		case 0: // weapon
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_WEAPON && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 1: // body
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_BODY && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 2:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_EAR && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 3:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_FOOTS && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 4:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_HEAD && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 5:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_NECK && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 6:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_SHIELD && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 7:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_WRIST && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 8: // chests
			if (it->second->GetType() == CItemData::ITEM_TYPE_GIFTBOX)
				m_tempItemVec.push_back(it->first);
			break;
		case 9: // belts
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_BELT)
				m_tempItemVec.push_back(it->first);
			break;
		case 10: // talisman
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_PENDANT && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		}
	}

	return m_tempItemVec.size();
}

std::tuple<const char*, _wint32> CItemManager::SelectByNamePart(const char* namePart)
{
	char searchName[CItemData::ITEM_NAME_MAX_LEN + 1];
	memcpy(searchName, namePart, sizeof(searchName));
	for (_wint32 j = 0; j < sizeof(searchName); j++)
		searchName[j] = tolower(searchName[j]);
	std::string tempSearchName = searchName;

	for (TItemMap::iterator i = m_ItemMap.begin(); i != m_ItemMap.end(); i++)
	{
		const CItemData::TItemTable* tbl = i->second->GetTable();

		if (!i->second->IsBlacklisted())
		{
			DWORD StartRefineVnum = GetWikiItemStartRefineVnum(i->first);
			if (StartRefineVnum != 0)
			{
				CItemData* _sRb = nullptr;
				if (!GetItemDataPointer(StartRefineVnum, &_sRb))
					continue;

				if (_sRb->IsBlacklisted())
					continue;
			}
		}
		else
			continue;

		char itemName[CItemData::ITEM_NAME_MAX_LEN + 1];
		memcpy(itemName, tbl->szLocaleName, sizeof(itemName));
		for (_wint32 j = 0; j < sizeof(itemName); j++)
			itemName[j] = tolower(itemName[j]);
		std::string tempName = itemName;

		const size_t tempSearchNameLenght = tempSearchName.length();
		if (tempName.length() < tempSearchNameLenght)
			continue;

		if (!tempName.substr(0, tempSearchNameLenght).compare(tempSearchName))
			return std::make_tuple(tbl->szLocaleName, tbl->dwVnum);
	}

	return std::make_tuple("", -1);
}
#endif


CItemManager::CItemManager() : m_pSelectedItemData(NULL)
{
}

CItemManager::~CItemManager()
{
	Destroy();
}

#ifdef ENABLE_CHEST_DROP
bool CItemManager::LoadChestDropInfo(const char* c_szFileName)
{
	if (m_ItemDropInfoMap.empty() == false)
		return true;

	CMappedFile file;
	LPCVOID pvData;

	if (!CEterPackManager::Instance().Get(file, c_szFileName, &pvData))
		return false;

	size_t mapSize = 0;
	file.Read(&mapSize, sizeof(mapSize));

	for (size_t i = 0; i < mapSize; i++)
	{
		DWORD dwItemVnum = 0;
		file.Read(&dwItemVnum, sizeof(dwItemVnum));

		size_t vecSize = 0;
		file.Read(&vecSize, sizeof(vecSize));

		TChestDropItemInfoVec& vecDrop = m_ItemDropInfoMap[dwItemVnum];
		vecDrop.reserve(vecSize);

		for (size_t j = 0; j < vecSize; j++)
		{
			DWORD dwDropVnum = 0;
			file.Read(&dwDropVnum, sizeof(dwDropVnum));

			int iCount = 0;
			file.Read(&iCount, sizeof(iCount));

			vecDrop.push_back({ dwDropVnum, iCount });
		}

		std::sort(vecDrop.begin(), vecDrop.end(),
			[this](const SDropItemInfo& a, const SDropItemInfo& b)
			{
				CItemData* pItemData[2];
				if (GetItemDataPointer(a.dwDropVnum, &pItemData[0]) && GetItemDataPointer(b.dwDropVnum, &pItemData[1]))
					return pItemData[0]->GetSize() < pItemData[1]->GetSize();

				return false;
			});
	}

	return true;
}

const CItemManager::TChestDropItemInfoVec* CItemManager::GetItemDropInfoVec(const DWORD dwVnum) const
{
	TChestDropItemInfoMap::const_iterator it = m_ItemDropInfoMap.find(dwVnum);
	if (it != m_ItemDropInfoMap.end())
	{
		const TChestDropItemInfoVec* pVec = &(it->second);
		return pVec;
	}

	return nullptr;
}
#endif
