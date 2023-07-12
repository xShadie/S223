#include "Mysql2Proto.h"

static bool s_bDebugFlag = false;

static const char* s_szdatabase = nullptr;
static const char* s_szhostname = nullptr;
static const char* s_szuser = nullptr;
static const char* s_szpassword = nullptr;
static unsigned int s_szport = 0;

static const char* s_szlocale_name = "locale_name";
static const char* s_szpostfix = "";

static CPythonNonPlayer::TMobTable* s_pMobTable = nullptr;
static int s_iMobTableSize = 0;

static CItemData::TItemTable* s_pItemTable = nullptr;
static int s_iItemTableSize = 0;

static DWORD g_adwMobProtoKey[4] =
{
	4813894,
	18955,
	552631,
	6822045
};

static DWORD g_adwItemProtoKey[4] =
{
	173217,
	72619434,
	408587239,
	27973291
};

MysqlWrapper zMWObj;
#pragma comment(lib, "libmysql.lib")

static int retrieveAddonType(DWORD dwVnum)
{
	int addon_type = 0;
#ifdef ENABLE_ADDONTYPE_AUTODETECT
	static DWORD vnumlist[] = { 180, 190, 290, 1130, 1170, 2150, 2170, 3160, 3210, 5110, 5120, 7160, 6010, 6060, 6070 };
	for (DWORD i = 0; i < (sizeof(vnumlist) / sizeof(DWORD)); i++)
	{
		if ((dwVnum >= vnumlist[i]) && (dwVnum <= vnumlist[i] + 9))
		{
			addon_type = -1;
		}
	}
#endif
	return addon_type;
}

static bool BuildMobTable()
{
	fprintf(stdout, "sizeof(CPythonNonPlayer::TMobTable): %u\n", sizeof(CPythonNonPlayer::TMobTable));

	if (s_pMobTable)
	{
		delete s_pMobTable;
		s_pMobTable = NULL;
	}

	char query[2048];
	fprintf(stdout, "Loading mob_proto from MySQL\n");
	int iQueryLen = snprintf(query, sizeof(query), "SELECT vnum, name, "
		"locale_name, rank, type, battle_type, level, scalepct, "
		"size+0, ai_flag+0, mount_capacity, setRaceFlag+0, setImmuneFlag+0, "
		"empire, folder, on_click, "
		"st, dx, ht, iq, damage_min, damage_max, max_hp, regen_cycle, regen_percent, gold_min, "
		"gold_max, exp, def, attack_speed, move_speed, "
		"aggressive_hp_pct, aggressive_sight, attack_range, drop_item, resurrection_vnum, "
		"enchant_curse, enchant_slow, enchant_poison, enchant_stun, enchant_critical, enchant_penetrate, "
		"resist_sword, resist_twohand, resist_dagger, resist_bell, resist_fan, resist_bow, "
#ifdef ENABLE_WOLFMAN
		" resist_claw,"
#endif
		" resist_fire, resist_elect, resist_magic, resist_wind, resist_poison,"
#ifdef ENABLE_WOLFMAN
		"resist_bleeding,"
#endif
		"dam_multiply, summon, drain_sp, mob_color, polymorph_item "
		"skill_vnum0, skill_level0, skill_vnum1, skill_level1, skill_vnum2, skill_level2,"
		"skill_vnum3, skill_level3, skill_vnum4, skill_level4 , sp_berserk, sp_stoneskin, "
		"sp_godspeed, sp_deathblow, sp_revive "
		"FROM mob_proto%s ORDER BY vnum",
		s_szpostfix);

	MysqlResSP pMsg(MysqlWrapper::instance().ResultQuery(query));
	MYSQL_RES* res = pMsg.Get();
	s_iMobTableSize = MysqlWrapper::instance().CountRows(res);

	s_pMobTable = new CPythonNonPlayer::TMobTable[s_iMobTableSize];
	memset(s_pMobTable, 0, sizeof(CPythonNonPlayer::TMobTable) * s_iMobTableSize);

	CPythonNonPlayer::TMobTable* pMobTable = s_pMobTable;

	int col;
	MYSQL_ROW data = NULL;
	while ((data = MysqlWrapper::instance().FetchRow(res)))
	{
		col = 0;
		str_to_number(pMobTable->dwVnum, data[col++]);
		strncpy_s(pMobTable->szName, data[col++], sizeof(pMobTable->szName));
		strncpy_s(pMobTable->szLocaleName, data[col++], sizeof(pMobTable->szLocaleName));	
		str_to_number(pMobTable->bRank, data[col++]);
		str_to_number(pMobTable->bType, data[col++]);
		str_to_number(pMobTable->bBattleType, data[col++]);
		str_to_number(pMobTable->bLevel, data[col++]);
		str_to_number(pMobTable->bScale, data[col++]);
		str_to_number(pMobTable->bSize, data[col++]);
		str_to_number(pMobTable->dwAIFlag, data[col++]);
		str_to_number(pMobTable->bMountCapacity, data[col++]);
		str_to_number(pMobTable->dwRaceFlag, data[col++]);
		str_to_number(pMobTable->dwImmuneFlag, data[col++]);
		str_to_number(pMobTable->bEmpire, data[col++]);
		strncpy_s(pMobTable->szFolder, data[col++], sizeof(pMobTable->szFolder));
		str_to_number(pMobTable->bOnClickType, data[col++]);
		str_to_number(pMobTable->bStr, data[col++]);
		str_to_number(pMobTable->bDex, data[col++]);
		str_to_number(pMobTable->bCon, data[col++]);
		str_to_number(pMobTable->bInt, data[col++]);
		str_to_number(pMobTable->dwDamageRange[0], data[col++]);
		str_to_number(pMobTable->dwDamageRange[1], data[col++]);
		str_to_number(pMobTable->dwMaxHP, data[col++]);
		str_to_number(pMobTable->bRegenCycle, data[col++]);
		str_to_number(pMobTable->bRegenPercent, data[col++]);
		str_to_number(pMobTable->dwGoldMin, data[col++]);
		str_to_number(pMobTable->dwGoldMax, data[col++]);
		str_to_number(pMobTable->dwExp, data[col++]);
		str_to_number(pMobTable->wDef, data[col++]);
		str_to_number(pMobTable->sAttackSpeed, data[col++]);
		str_to_number(pMobTable->sMovingSpeed, data[col++]);
		str_to_number(pMobTable->bAggresiveHPPct, data[col++]);
		str_to_number(pMobTable->wAggressiveSight, data[col++]);
		str_to_number(pMobTable->wAttackRange, data[col++]);
		str_to_number(pMobTable->dwDropItemVnum, data[col++]);
		str_to_number(pMobTable->dwResurrectionVnum, data[col++]);

		int i;
		for (i = 0; i < CPythonNonPlayer::MOB_ENCHANTS_MAX_NUM; ++i)
			str_to_number(pMobTable->cEnchants[i], data[col++]);

		for (i = 0; i < CPythonNonPlayer::MOB_RESISTS_MAX_NUM; ++i)
			str_to_number(pMobTable->cResists[i], data[col++]);

		str_to_number(pMobTable->fDamMultiply, data[col++]);
		str_to_number(pMobTable->dwSummonVnum, data[col++]);
		str_to_number(pMobTable->dwDrainSP, data[col++]);
		str_to_number(pMobTable->dwMonsterColor, data[col++]);
		str_to_number(pMobTable->dwPolymorphItemVnum, data[col++]);

		for (i = 0; i < MOB_SKILL_MAX_NUM; ++i)
		{
			str_to_number(pMobTable->Skills[i].dwVnum, data[col++]);
			str_to_number(pMobTable->Skills[i].bLevel, data[col++]);
		}

		str_to_number(pMobTable->bBerserkPoint, data[col++]);
		str_to_number(pMobTable->bStoneSkinPoint, data[col++]);
		str_to_number(pMobTable->bGodSpeedPoint, data[col++]);
		str_to_number(pMobTable->bDeathBlowPoint, data[col++]);
		str_to_number(pMobTable->bRevivePoint, data[col++]);
		++pMobTable;
	}

	fprintf(stdout, "Complete! %i Mobs loaded.\n", s_iMobTableSize);
	return true;
}

static void SaveMobProto()
{
	if (!s_pMobTable)
		return;

	FILE* fp;
	fopen_s(&fp, "mob_proto", "wb");
	if (!fp)
	{
		printf("cannot open %s for writing\n", "mob_proto");
		return;
	}

	DWORD fourcc = MAKEFOURCC('M', 'M', 'P', 'T');
	fwrite(&fourcc, sizeof(DWORD), 1, fp);

	DWORD dwElements = s_iMobTableSize;
	fwrite(&dwElements, sizeof(DWORD), 1, fp);

	CLZObject zObj;

	if (!CLZO::instance().CompressEncryptedMemory(zObj, s_pMobTable, sizeof(CPythonNonPlayer::TMobTable) * s_iMobTableSize, g_adwMobProtoKey))
	{
		printf("cannot compress\n");
		fclose(fp);
		return;
	}

	const CLZObject::THeader& r = zObj.GetHeader();

	printf("MobProto count %u\n%u --Compress--> %u --Encrypt--> %u, GetSize %u\n",
		s_iMobTableSize, r.dwRealSize, r.dwCompressedSize, r.dwEncryptSize, zObj.GetSize());

	DWORD dwDataSize = zObj.GetSize();
	fwrite(&dwDataSize, sizeof(DWORD), 1, fp);
	fwrite(zObj.GetBuffer(), dwDataSize, 1, fp);

	fclose(fp);
}

static bool BuildItemTable()
{
	fprintf(stdout, "sizeof(CItemData::TItemTable): %u\n", sizeof(CItemData::TItemTable));

	if (s_pItemTable)
	{
		free(s_pItemTable);
		s_pItemTable = NULL;
	}

	char query[2048];
	fprintf(stdout, "Loading item_proto from MySQL\n");
	int iQueryLen = snprintf(query, sizeof(query), "SELECT vnum, vnum_range, name, "
		" locale_name, type, subtype, weight, size, antiflag, flag, "
		" wearflag, immuneflag+0, gold, shop_buy_price, refined_vnum, refine_set, magic_pct, "
		" limittype0, limitvalue0, limittype1, limitvalue1, "
		" applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, "
		" value0, value1, value2, value3, value4, value5, specular, socket_pct, addon_type "
		" FROM item_proto%s ORDER BY vnum",
		s_szpostfix);

	MysqlResSP pMsg(MysqlWrapper::instance().ResultQuery(query));
	MYSQL_RES* res = pMsg.Get();

	s_iItemTableSize = MysqlWrapper::instance().CountRows(res);

	s_pItemTable = new CItemData::TItemTable[s_iItemTableSize];
	memset(s_pItemTable, 0, sizeof(CItemData::TItemTable) * s_iItemTableSize);

	CItemData::TItemTable* pItemTable = s_pItemTable;

	int col;
	MYSQL_ROW data = NULL;
	while ((data = MysqlWrapper::instance().FetchRow(res)))
	{
		col = 0;

		str_to_number(pItemTable->dwVnum, data[col++]);
		str_to_number(pItemTable->dwVnumRange, data[col++]);
		strncpy_s(pItemTable->szName, data[col++], sizeof(pItemTable->szName));
		strncpy_s(pItemTable->szLocaleName, data[col++], sizeof(pItemTable->szLocaleName));
		str_to_number(pItemTable->bType, data[col++]);
		str_to_number(pItemTable->bSubType, data[col++]);
		str_to_number(pItemTable->bWeight, data[col++]);
		str_to_number(pItemTable->bSize, data[col++]);
		str_to_number(pItemTable->dwAntiFlags, data[col++]);
		str_to_number(pItemTable->dwFlags, data[col++]);
		str_to_number(pItemTable->dwWearFlags, data[col++]);
		str_to_number(pItemTable->dwImmuneFlag, data[col++]);
		str_to_number(pItemTable->dwIBuyItemPrice, data[col++]);
		str_to_number(pItemTable->dwISellItemPrice, data[col++]);
		str_to_number(pItemTable->dwRefinedVnum, data[col++]);
		str_to_number(pItemTable->wRefineSet, data[col++]);
		str_to_number(pItemTable->bAlterToMagicItemPct, data[col++]);

		int i;
		for (i = 0; i < CItemData::ITEM_LIMIT_MAX_NUM; ++i)
		{
			str_to_number(pItemTable->aLimits[i].bType, data[col++]);
			str_to_number(pItemTable->aLimits[i].lValue, data[col++]);
		}

		for (i = 0; i < CItemData::ITEM_APPLY_MAX_NUM; ++i)
		{
			str_to_number(pItemTable->aApplies[i].bType, data[col++]);
			str_to_number(pItemTable->aApplies[i].lValue, data[col++]);
		}

		for (i = 0; i < CItemData::ITEM_VALUES_MAX_NUM; ++i)
			str_to_number(pItemTable->alValues[i], data[col++]);

	//	for (i = 0; i < CItemData::ITEM_SOCKET_MAX_NUM; ++i)
	//		str_to_number(pItemTable->alSockets[i], data[col++]);

		str_to_number(pItemTable->bSpecular, data[col++]);
		str_to_number(pItemTable->bGainSocketPct, data[col++]);

		if (pItemTable->bType == CItemData::ITEM_TYPE_DS)
			pItemTable->dwVnumRange = 99;

		++pItemTable;
	}

	fprintf(stdout, "Complete! %u Items loaded.\n", s_iItemTableSize);
	return true;
}

static void SaveItemProto()
{
	if (!s_pItemTable)
		return;

	FILE* fp;
	fopen_s(&fp, "item_proto", "wb");
	if (!fp)
	{
		printf("cannot open %s for writing\n", "item_proto");
		return;
	}

	DWORD fourcc = MAKEFOURCC('M', 'I', 'P', 'X');
	fwrite(&fourcc, sizeof(DWORD), 1, fp);

	DWORD dwVersion = 0x00000001;
	fwrite(&dwVersion, sizeof(DWORD), 1, fp);

	DWORD dwStride = sizeof(CItemData::TItemTable);
	fwrite(&dwStride, sizeof(DWORD), 1, fp);

	DWORD dwElements = s_iItemTableSize;
	fwrite(&dwElements, sizeof(DWORD), 1, fp);

	CLZObject zObj;

	std::vector <CItemData::TItemTable> vec_item_table(&s_pItemTable[0], &s_pItemTable[s_iItemTableSize - 1]);
	std::sort(&s_pItemTable[0], &s_pItemTable[0] + s_iItemTableSize);
	if (!CLZO::instance().CompressEncryptedMemory(zObj, s_pItemTable, sizeof(CItemData::TItemTable) * s_iItemTableSize, g_adwItemProtoKey))
	{
		printf("cannot compress\n");
		fclose(fp);
		return;
	}

	const CLZObject::THeader& r = zObj.GetHeader();

	printf("Elements %d\n%u --Compress--> %u --Encrypt--> %u, GetSize %u\n",
		s_iItemTableSize,
		r.dwRealSize,
		r.dwCompressedSize,
		r.dwEncryptSize,
		zObj.GetSize());

	DWORD dwDataSize = zObj.GetSize();
	fwrite(&dwDataSize, sizeof(DWORD), 1, fp);
	fwrite(zObj.GetBuffer(), dwDataSize, 1, fp);
	fclose(fp);

	fopen_s(&fp, "item_proto", "rb");
	if (!fp)
	{
		printf("Error!!\n");
		return;
	}

	fread(&fourcc, sizeof(DWORD), 1, fp);
	fread(&dwElements, sizeof(DWORD), 1, fp);

	printf("Elements Check %u fourcc match %d\n", dwElements, fourcc == MAKEFOURCC('M', 'I', 'P', 'T'));
	fclose(fp);
}

static bool LoadConfig()
{
	const char* jsonFName = "Mysql2Proto.json";
	if (!FileExists(jsonFName))
	{
		fprintf(stderr, "<ConfigFile> LOAD '%s' FAILED (NO_FILE)\n", jsonFName);
		return false;
	}

	std::string jsonData = GetFileContent(jsonFName);

	rapidjson::Document document;
	if (document.Parse(jsonData.c_str()).HasParseError())
	{
		fprintf(stderr, "<ConfigFile> LOAD '%s' FAILED (PARSE_ERROR)\n", jsonFName);
		return false;
	}

	fprintf(stdout, "<ConfigFile> LOAD '%s' BEGIN\n", jsonFName);

	if (document.HasMember("database"))
	{
		const rapidjson::Value& value = document["database"];
		assert(value.IsString());
		s_szdatabase = value.GetString();
		fprintf(stdout, "<ConfigFile> s_szdatabase changed to '%s'.\n", s_szdatabase);
	}
	else
	{
		fprintf(stderr, "<ConfigFile> LOAD '%s' FAILED (database VALUE MISSING)\n", jsonFName);
		return false;
	}

	if (document.HasMember("hostname"))
	{
		const rapidjson::Value& value = document["hostname"];
		assert(value.IsString());
		s_szhostname = value.GetString();
		fprintf(stdout, "<ConfigFile> s_szhostname changed to '%s'.\n", s_szhostname);
	}
	else
	{
		fprintf(stderr, "<ConfigFile> LOAD '%s' FAILED (hostname VALUE MISSING)\n", jsonFName);
		return false;
	}

	if (document.HasMember("user"))
	{
		const rapidjson::Value& value = document["user"];
		assert(value.IsString());
		s_szuser = value.GetString();
		fprintf(stdout, "<ConfigFile> s_szuser changed to '%s'.\n", s_szuser);
	}
	else
	{
		fprintf(stderr, "<ConfigFile> LOAD '%s' FAILED (user VALUE MISSING)\n", jsonFName);
		return false;
	}

	if (document.HasMember("password"))
	{
		const rapidjson::Value& value = document["password"];
		assert(value.IsString());
		s_szpassword = value.GetString();
		fprintf(stdout, "<ConfigFile> s_szpassword changed to '%s'.\n", s_szpassword);
	}
	else
	{
		fprintf(stderr, "<ConfigFile> LOAD '%s' FAILED (password VALUE MISSING)\n", jsonFName);
		return false;
	}

	if (document.HasMember("port"))
	{
		const rapidjson::Value& value = document["port"];
		assert(value.IsNumber());
		s_szport = value.GetInt();
		fprintf(stdout, "<ConfigFile> s_szport changed to '%u'.\n", s_szport);
	}

	fprintf(stdout, "<ConfigFile> LOAD '%s' END\n", jsonFName);
	MysqlWrapper::instance().Connect(s_szhostname, s_szuser, s_szpassword, s_szdatabase, s_szport);
	fprintf(stdout, "<ConfigFile> MYSQL SUCCESSFULLY CONNECTED\n", jsonFName);
	return true;
}

int main()
{
	if (!LoadConfig())
		exit(0);

#ifdef _DEBUG
	printf("sizeof(TItemTable) %d\n", sizeof(CItemData::TItemTable));
	printf("sizeof(TMobTable) %d\n", sizeof(CPythonNonPlayer::TMobTable));
#endif

	if (BuildItemTable())
		SaveItemProto();

	if (BuildMobTable())
		SaveMobProto();

	system("Pause");
	return 0;
}
