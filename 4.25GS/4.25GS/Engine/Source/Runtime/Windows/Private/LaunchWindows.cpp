#include "pch.h"

static void InstallDetours()
{
	InstallDetour(Offsets::GetNetModeWorld, GetNetModeWorldHook);
	InstallDetour(Offsets::GetNetModeActor, GetNetModeActorHook);
	InstallDetour(Offsets::KickPlayer, KickPlayerHook, &KickPlayerOriginal);
}

static void InstallTickHook()
{
	InstallDetour(Offsets::TickFlush, TickFlushHook, &TickFlushOriginal);
}

static void InstallVirtualHooks()
{
	auto* GameModeDefault = FindObject<AFortGameModeAthena>("/Script/FortniteGame.Default__FortGameModeAthena");
	auto* ControllerDefault = FindObject<AFortPlayerControllerAthena>("/Script/FortniteGame.Default__FortPlayerControllerAthena");
	auto* AbilityDefault = GetDefaultObject<UFortAbilitySystemComponentAthena>();

	InstallVirtual(GameModeDefault, VTable::ReadyToStartMatch, ReadyToStartMatchHook, &ReadyToStartMatchOriginal);
	InstallVirtual(GameModeDefault, VTable::SpawnDefaultPawnFor, SpawnDefaultPawnForHook);
	InstallVirtual(GameModeDefault, VTable::HandleStartingNewPlayer, HandleStartingNewPlayerHook, &HandleStartingNewPlayerOriginal);
	InstallVirtual(ControllerDefault, VTable::ServerExecuteInventoryItem, ServerExecuteInventoryItemHook);
	InstallVirtual(AbilityDefault, VTable::InternalServerTryActivateAbility, InternalServerTryActivateAbilityHook);
}

static DWORD WINAPI Main(LPVOID)
{
	AllocConsole();

	FILE* Console = nullptr;
	freopen_s(&Console, "CONOUT$", "w+", stdout);

	SetConsoleTitleA("4.25 Gameserver");

	MH_Initialize();
	Log("MinHook ready");

	InitGObjects();
	Log("GObjects ready");

	InstallDetours();
	Log("Detours installed");

	LoadStartupMap();
	Log("Startup map requested");

	DisableEngineFunctions();
	Log("Engine functions disabled");

	InstallVirtualHooks();
	Log("Virtual hooks installed");

	InstallTickHook();
	Log("Tick hook installed");

	return 0;
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, Main, nullptr, 0, nullptr);

	return TRUE;
}
