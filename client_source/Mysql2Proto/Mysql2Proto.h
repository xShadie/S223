#pragma once
#define _WINSOCKAPI_
#define _USE_32BIT_TIME_T

#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <list>

#pragma comment(lib, "lzo-1.08MD.lib")
#ifndef _DEBUG
#pragma comment(lib, "liblz4/release/liblz4_static.lib")
#else
#pragma comment(lib, "liblz4/debug/liblz4_static.lib")
#endif

#include "../EterBase/Singleton.h"
#include "../GameLib/StdAfx.h"
#include "../GameLib/ItemData.h"

#include "../UserInterface/StdAfx.h"
#include "../UserInterface/PythonNonPlayer.h"
#include "../UserInterface/Defines.h"

inline bool operator<(const CItemData::TItemTable& lhs, const CItemData::TItemTable& rhs)
{
	return lhs.dwVnum < rhs.dwVnum;
}

#include "MysqlWrapper.h"
#include "utils.h"

#define VERIFY_IFIELD(x,y) if (data[x]!=NULL && data[x][0]!='\0') str_to_number(y, data[x]);
#define VERIFY_SFIELD(x,y) if (data[x]!=NULL && data[x][0]!='\0') strncpy_s(y, sizeof(y), data[x], _TRUNCATE);

#define ENABLE_LIMIT_TYPE_CHECK_FIX
#define ENABLE_ADDONTYPE_AUTODETECT

#include <fstream>
inline std::string GetFileContent(const std::string& path)
{
	std::ifstream file(path);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

inline bool FileExists(const std::string& name)
{
	FILE* fp;
	fopen_s(&fp, name.c_str(), "r");
	if (fp)
	{
		fclose(fp);
		return true;
	}
	else
	{
		return false;
	}
}

#include "rapidjson/document.h"

#define NL "\n"
#include "OptionParser/OptionParser.h"
