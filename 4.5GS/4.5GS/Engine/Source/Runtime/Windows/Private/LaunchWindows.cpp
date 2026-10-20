#include "pch.h"

static void LogAddress(const char* Label, uintptr_t Address)
{
	HMODULE Module = nullptr;
	char Name[MAX_PATH] = "unknown";
	uintptr_t Base = 0;

	if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCSTR>(Address), &Module) && Module)
	{
		GetModuleFileNameA(Module, Name, MAX_PATH);
		Base = reinterpret_cast<uintptr_t>(Module);
	}

	char Line[512];
	sprintf_s(Line, "%s 0x%llX = %s + 0x%llX", Label, static_cast<unsigned long long>(Address), Name, static_cast<unsigned long long>(Address - Base));
	Log(Line);
}

static LONG WINAPI CrashLogger(EXCEPTION_POINTERS* Info)
{
	const DWORD Code = Info->ExceptionRecord->ExceptionCode;

	if (Code != EXCEPTION_ACCESS_VIOLATION && Code != EXCEPTION_ILLEGAL_INSTRUCTION && Code != EXCEPTION_STACK_OVERFLOW)
		return EXCEPTION_CONTINUE_SEARCH;

	char Line[256];
	sprintf_s(Line, "Exception 0x%08lX, access type %llu, target 0x%llX", Code, static_cast<unsigned long long>(Info->ExceptionRecord->ExceptionInformation[0]), static_cast<unsigned long long>(Info->ExceptionRecord->ExceptionInformation[1]));
	Log(Line);

	LogAddress("Fault at", reinterpret_cast<uintptr_t>(Info->ExceptionRecord->ExceptionAddress));

	void* Frames[24] = {};
	const USHORT Count = RtlCaptureStackBackTrace(0, 24, Frames, nullptr);

	for (USHORT Index = 0; Index < Count; Index++)
		LogAddress("Frame", reinterpret_cast<uintptr_t>(Frames[Index]));

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

	SetConsoleTitleA("4.5 Gameserver");

	AddVectoredExceptionHandler(1, CrashLogger);
	Log("GS started");

	MH_Initialize();
	Log("MinHook ready");

	LogBuildFingerprint();

	InitGObjects();
	LogObjectArray();

	if (!ObjectArrayLooksValid())
	{
		Log("SDK GObjects offset is invalid, scanning for the object array");

		TUObjectArray* Found = ScanForObjectArray();

		if (!Found)
		{
			Log("Object array not found, stopping");
			return 0;
		}

		char Line[160];
		sprintf_s(Line, "Object array found at image base + 0x%llX", static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(Found) - ImageBase()));
		Log(Line);

		UObject::GObjects = Found;
		LogObjectArray();
	}

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
