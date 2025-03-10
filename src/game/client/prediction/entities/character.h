/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_CLIENT_PREDICTION_ENTITIES_CHARACTER_H
#define GAME_CLIENT_PREDICTION_ENTITIES_CHARACTER_H

#include "projectile.h"
#include <game/client/prediction/entity.h>

#include <game/gamecore.h>
#include <game/generated/client_data.h>

enum
{
	WEAPON_GAME = -3, // team switching etc
	WEAPON_SELF = -2, // console kill command
	WEAPON_WORLD = -1, // death tiles etc
};

enum
{
	FAKETUNE_FREEZE = 1,
	FAKETUNE_SOLO = 2,
	FAKETUNE_NOJUMP = 4,
	FAKETUNE_NOCOLL = 8,
	FAKETUNE_NOHOOK = 16,
	FAKETUNE_JETPACK = 32,
	FAKETUNE_NOHAMMER = 64,

};

class CCharacter : public CEntity
{
	friend class CGameWorld;

public:
	//character's size
	static const int ms_PhysSize = 28;

	virtual void Tick();
	virtual void TickDefered();

	bool IsGrounded();

	void SetWeapon(int W);
	void SetSolo(bool Solo);
	void HandleWeaponSwitch();
	void DoWeaponSwitch();

	void HandleWeapons();
	void HandleNinja();
	void HandleJetpack();

	void OnPredictedInput(CNetObj_PlayerInput *pNewInput);
	void OnDirectInput(CNetObj_PlayerInput *pNewInput);
	void ResetInput();
	void FireWeapon();

	bool TakeDamage(vec2 Force, int Dmg, int From, int Weapon);

	void GiveWeapon(int Weapon, bool Remove = false);
	void GiveNinja();
	void RemoveNinja();

	bool IsAlive() { return m_Alive; }

	bool m_Alive;
	bool m_IsLocal;

	CTeamsCore *TeamsCore();
	bool Freeze(int Seconds);
	bool Freeze();
	bool UnFreeze();
	void GiveAllWeapons();
	int Team();
	bool CanCollide(int ClientID);
	bool SameTeam(int ClientID);
	bool m_Super;
	bool m_SuperJump;
	bool m_Jetpack;
	bool m_NinjaJetpack;
	int m_FreezeTime;
	int m_FreezeTick;
	bool m_FrozenLastTick;
	bool m_DeepFreeze;
	bool m_LiveFreeze;
	bool m_EndlessHook;
	enum
	{
		HIT_ALL = 0,
		DISABLE_HIT_HAMMER = 1,
		DISABLE_HIT_SHOTGUN = 2,
		DISABLE_HIT_GRENADE = 4,
		DISABLE_HIT_LASER = 8
	};
	int m_Hit;
	int m_TuneZone;
	vec2 m_PrevPos;
	vec2 m_PrevPrevPos;
	int m_TeleCheckpoint;

	int m_TileIndex;
	int m_TileFIndex;

	int m_MoveRestrictions;
	bool m_LastRefillJumps;

	// Setters/Getters because i don't want to modify vanilla vars access modifiers
	int GetLastWeapon() { return m_LastWeapon; };
	void SetLastWeapon(int LastWeap) { m_LastWeapon = LastWeap; };
	int GetActiveWeapon() { return m_Core.m_ActiveWeapon; };
	void SetActiveWeapon(int ActiveWeap);
	CCharacterCore GetCore() { return m_Core; };
	void SetCore(CCharacterCore Core) { m_Core = Core; };
	CCharacterCore *Core() { return &m_Core; };
	bool GetWeaponGot(int Type) { return m_aWeapons[Type].m_Got; };
	void SetWeaponGot(int Type, bool Value) { m_aWeapons[Type].m_Got = Value; };
	int GetWeaponAmmo(int Type) { return m_aWeapons[Type].m_Ammo; };
	void SetWeaponAmmo(int Type, int Value) { m_aWeapons[Type].m_Ammo = Value; };
	void SetNinjaActivationDir(vec2 ActivationDir) { m_Ninja.m_ActivationDir = ActivationDir; };
	void SetNinjaActivationTick(int ActivationTick) { m_Ninja.m_ActivationTick = ActivationTick; };
	void SetNinjaCurrentMoveTime(int CurrentMoveTime) { m_Ninja.m_CurrentMoveTime = CurrentMoveTime; };
	int GetCID() { return m_ID; }
	void SetInput(CNetObj_PlayerInput *pNewInput)
	{
		m_LatestInput = m_Input = *pNewInput;
		// it is not allowed to aim in the center
		if(m_Input.m_TargetX == 0 && m_Input.m_TargetY == 0)
		{
			m_Input.m_TargetY = m_LatestInput.m_TargetY = -1;
		}
	};
	int GetJumped() { return m_Core.m_Jumped; }
	int GetAttackTick() { return m_AttackTick; }
	int GetStrongWeakID() { return m_StrongWeakID; }

	CCharacter(CGameWorld *pGameWorld, int ID, CNetObj_Character *pChar, CNetObj_DDNetCharacter *pExtended = 0);
	void Read(CNetObj_Character *pChar, CNetObj_DDNetCharacter *pExtended, bool IsLocal);
	void SetCoreWorld(CGameWorld *pGameWorld);

	int m_LastSnapWeapon;
	int m_LastJetpackStrength;
	bool m_KeepHooked;
	int m_GameTeam;
	bool m_CanMoveInFreeze;

	bool Match(CCharacter *pChar);
	void ResetPrediction();
	CCharacter() { m_Alive = false; }
	void SetTuneZone(int Zone);

private:
	// weapon info
	int m_aHitObjects[10];
	int m_NumObjectsHit;

	struct WeaponStat
	{
		int m_AmmoRegenStart;
		int m_Ammo;
		int m_Ammocost;
		bool m_Got;
	} m_aWeapons[NUM_WEAPONS];

	int m_LastWeapon;
	int m_QueuedWeapon;

	int m_ReloadTimer;
	int m_AttackTick;

	// these are non-heldback inputs
	CNetObj_PlayerInput m_LatestPrevInput;
	CNetObj_PlayerInput m_LatestInput;

	// input
	CNetObj_PlayerInput m_PrevInput;
	CNetObj_PlayerInput m_Input;
	CNetObj_PlayerInput m_SavedInput;

	int m_NumInputs;

	// ninja
	struct NinjaStat
	{
		vec2 m_ActivationDir;
		int m_ActivationTick;
		int m_CurrentMoveTime;
		int m_OldVelAmount;
	} m_Ninja;

	// the player core for the physics
	CCharacterCore m_Core;

	// DDRace

	static bool IsSwitchActiveCb(int Number, void *pUser);
	void HandleTiles(int Index);
	void HandleSkippableTiles(int Index);
	void DDRaceTick();
	void DDRacePostCoreTick();
	void HandleTuneLayer();

	CTuningParams *CharacterTuning();

	int m_StrongWeakID;

	int m_LastWeaponSwitchTick;
	int m_LastTuneZoneTick;
};

enum
{
	DDRACE_NONE = 0,
	DDRACE_STARTED,
	DDRACE_CHEAT, // no time and won't start again unless ordered by a mod or death
	DDRACE_FINISHED
};

#endif
