#include "pch.h"

static LONG WINAPI CrashLogger(EXCEPTION_POINTERS* Info)
{
	const DWORD Code = Info->ExceptionRecord->ExceptionCode;

	if (Code == EXCEPTION_ACCESS_VIOLATION || Code == EXCEPTION_ILLEGAL_INSTRUCTION || Code == EXCEPTION_STACK_OVERFLOW)
	{
		const auto Address = reinterpret_cast<uintptr_t>(Info->ExceptionRecord->ExceptionAddress);

		char Line[256];
		sprintf_s(Line, "Exception 0x%08lX at 0x%llX (image base + 0x%llX)", Code, static_cast<unsigned long long>(Address), static_cast<unsigned long long>(Address - ImageBase()));
		Log(Line);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

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

	AddVectoredExceptionHandler(1, CrashLogger);
	Log("GS started");

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
