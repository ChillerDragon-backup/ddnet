/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#ifndef GAME_TEAMSCORE_H
#define GAME_TEAMSCORE_H

#include <engine/shared/protocol.h>

enum
{
	TEAM_FLOCK = 0,
	TEAM_SUPER = MAX_CLIENTS,
	NUM_TEAMS = TEAM_SUPER + 1,
	VANILLA_TEAM_SUPER = VANILLA_MAX_CLIENTS
};

class CTeamsCore
{
	int m_Team[MAX_CLIENTS];
	bool m_IsSolo[MAX_CLIENTS];

public:
	bool m_IsDDRace16;

	CTeamsCore();

	bool SameTeam(int ClientID1, int ClientID2) const;

	bool CanKeepHook(int ClientID1, int ClientID2) const;
	bool CanCollide(int ClientID1, int ClientID2) const;

	int Team(int ClientID) const;
	void Team(int ClientID, int Team);

	void Reset();
	void SetSolo(int ClientID, bool Value)
	{
		m_IsSolo[ClientID] = Value;
	}

	bool GetSolo(int ClientID) const
	{
		if(ClientID < 0 || ClientID >= MAX_CLIENTS)
			return false;
		return m_IsSolo[ClientID];
	}
};

#endif
