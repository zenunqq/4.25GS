#pragma once

#include <cstdint>

namespace Config
{
	constexpr int Port = 7777;
	constexpr int MaxPlayers = 100;
	constexpr int WarmupRequiredPlayers = 1;
	constexpr int BackpackSize = 5;
	constexpr int NetModeDedicatedServer = 1;

	constexpr const wchar_t* MapCommand = L"open Athena_Terrain";
	constexpr const wchar_t* AircraftCommand = L"startaircraft";
	constexpr const wchar_t* NetDriverName = L"GameNetDriver";

	constexpr const char* Playlist = "/Game/Athena/Playlists/Playlist_DefaultSolo.Playlist_DefaultSolo";
	constexpr const char* PawnClass = "/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C";
	constexpr const char* DefaultAbilitySet = "/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer";
	constexpr const char* RangedDamageAbility = "/Game/Abilities/Weapons/Ranged/GA_Ranged_GenericDamage.GA_Ranged_GenericDamage_C";
	constexpr const char* DefaultPickaxe = "/Game/Athena/Items/Cosmetics/Pickaxes/DefaultPickaxe.DefaultPickaxe";
	constexpr const char* DefaultHead = "/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1";
	constexpr const char* DefaultBody = "/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01";
}

namespace Offsets
{
	constexpr uintptr_t StaticFindObject = 0x15b6350;
	constexpr uintptr_t StaticLoadObject = 0x15b7e30;
	constexpr uintptr_t MarkArrayDirty = 0x340210;
	constexpr uintptr_t GiveAbility = 0x57fe50;
	constexpr uintptr_t InternalTryActivateAbility = 0x581590;
	constexpr uintptr_t InitHost = 0x39dab0;
	constexpr uintptr_t PauseBeaconRequests = 0xe15db0;
	constexpr uintptr_t SetWorld = 0x2298c60;
	constexpr uintptr_t InitListen = 0x39de90;
	constexpr uintptr_t TickFlush = 0x2299ff0;
	constexpr uintptr_t GetNetModeWorld = 0x2540030;
	constexpr uintptr_t GetNetModeActor = 0x1f4efd0;
	constexpr uintptr_t KickPlayer = 0x216dea0;

	constexpr uintptr_t Disabled[] = { 0xfb23f0, 0xc96ca0, 0xaec460 };
}

namespace VTable
{
	constexpr int ReadyToStartMatch = 253;
	constexpr int SpawnDefaultPawnFor = 193;
	constexpr int HandleStartingNewPlayer = 199;
	constexpr int ServerExecuteInventoryItem = 484;
	constexpr int InternalServerTryActivateAbility = 0xcb;
	constexpr int ReplicationDriverReplicate = 0x53;
}
