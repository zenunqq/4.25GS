#pragma once

inline uintptr_t ImageBase()
{
	return reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
}

template <typename Fn>
inline Fn Resolve(uintptr_t Offset)
{
	return reinterpret_cast<Fn>(ImageBase() + Offset);
}

inline void Log(const char* Message)
{
	std::cout << Message << std::endl;

	FILE* File = nullptr;

	if (fopen_s(&File, "GS.log", "a") == 0 && File)
	{
		fprintf(File, "%s\n", Message);
		fclose(File);
	}
}

inline bool IsReadable(const void* Address, size_t Size = 1)
{
	MEMORY_BASIC_INFORMATION Info{};

	if (!Address || !VirtualQuery(Address, &Info, sizeof(Info)))
		return false;

	if (Info.State != MEM_COMMIT || (Info.Protect & (PAGE_NOACCESS | PAGE_GUARD)))
		return false;

	const uintptr_t Start = reinterpret_cast<uintptr_t>(Address);
	const uintptr_t RegionEnd = reinterpret_cast<uintptr_t>(Info.BaseAddress) + Info.RegionSize;

	return Start + Size <= RegionEnd;
}

inline void LogBuildFingerprint()
{
	const uintptr_t Base = ImageBase();
	const auto* Dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(Base);
	const auto* Nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(Base + Dos->e_lfanew);

	char Line[160];
	sprintf_s(Line, "Build image size 0x%lX, timestamp 0x%lX", static_cast<unsigned long>(Nt->OptionalHeader.SizeOfImage), static_cast<unsigned long>(Nt->FileHeader.TimeDateStamp));
	Log(Line);
}
