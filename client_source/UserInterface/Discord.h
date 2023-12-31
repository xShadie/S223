#pragma once
#include "StdAfx.h"
#include "PythonCharacterManager.h"
#include "PythonBackground.h"
#include "PythonPlayer.h"
#include "PythonGuild.h"

namespace Discord
{
	const auto DiscordClientID = DISCORD_CLIENT_ID;

	using DCDATA = std::pair<std::string, std::string>;

	/*NAME*/
	DCDATA GetNameData()
	{
		char buffer[40];
		_snprintf(buffer, sizeof(buffer), "%s - Lv. %lld", CPythonPlayer::Instance().GetName(), CPythonPlayer::Instance().GetStatus(POINT_LEVEL));
		auto CHName = buffer;
		auto Homepage = DISCORD_HOMEPAGE_LINK;
		return { Homepage, CHName };
	}

	/*RACE*/
	DCDATA GetRaceData()
	{
		auto pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (!pInstance)
			return { "","" };

		auto RACENUM = pInstance->GetRace();

		/*Image*/
		auto RaceImage = "race_" + std::to_string(RACENUM);

		/*Name*/
		auto RaceName = "Warrior";
		switch (RACENUM)
		{
		case NRaceData::JOB_ASSASSIN:
		case NRaceData::JOB_ASSASSIN + 4:
			RaceName = "Assassin";
			break;
		case NRaceData::JOB_SURA:
		case NRaceData::JOB_SURA + 4:
			RaceName = "Sura";
			break;
		case NRaceData::JOB_SHAMAN:
		case NRaceData::JOB_SHAMAN + 4:
			RaceName = "Shaman";
			break;
		}
		return { RaceImage , RaceName };
	}

	/*EMPIRE*/
	DCDATA GetEmpireData()
	{
		auto pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (!pInstance)
			return { "","" };

		auto EmpireID = pInstance->GetEmpireID();

		if (pInstance->IsGameMaster())
			return { "empire_gm", "Team" };

		/*Image*/
		auto EmpireImage = "empire_" + std::to_string(EmpireID);

		/*Name*/
		auto EmpireName = "Shinsoo";
		switch (EmpireID)
		{
		case 2:
			EmpireName = "Chunjo";
			break;
		case 3:
			EmpireName = "Jinno";
		}
		return { EmpireImage, EmpireName };
	}
}