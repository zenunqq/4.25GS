#pragma once

template <typename Hook, typename Original = void*>
inline bool InstallDetour(uintptr_t Offset, Hook Detour, Original* OutOriginal = nullptr)
{
	void* Target = reinterpret_cast<void*>(ImageBase() + Offset);

	if (MH_CreateHook(Target, reinterpret_cast<void*>(Detour), reinterpret_cast<void**>(OutOriginal)) != MH_OK)
		return false;

	return MH_EnableHook(Target) == MH_OK;
}

template <typename Hook, typename Original = void*>
inline void InstallVirtual(UObject* DefaultObject, int Index, Hook Detour, Original* OutOriginal = nullptr)
{
	void** Table = DefaultObject->Vft;

	if (OutOriginal)
		*reinterpret_cast<void**>(OutOriginal) = Table[Index];

	DWORD OldProtect = 0;
	VirtualProtect(&Table[Index], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtect);
	Table[Index] = reinterpret_cast<void*>(Detour);
	VirtualProtect(&Table[Index], sizeof(void*), OldProtect, &OldProtect);
}

template <typename Hook, typename Original = void*>
inline void InstallExec(UFunction* Function, Hook Detour, Original* OutOriginal = nullptr)
{
	if (!Function)
		return;

	if (OutOriginal)
		*reinterpret_cast<void**>(OutOriginal) = reinterpret_cast<void*>(Function->ExecFunction);

	Function->ExecFunction = reinterpret_cast<UFunction::FNativeFuncPtr>(Detour);
}

inline void PatchReturn(uintptr_t Offset)
{
	auto* Address = reinterpret_cast<uint8_t*>(ImageBase() + Offset);

	DWORD OldProtect = 0;
	VirtualProtect(Address, 1, PAGE_EXECUTE_READWRITE, &OldProtect);
	*Address = 0xC3;
	VirtualProtect(Address, 1, OldProtect, &OldProtect);
}
